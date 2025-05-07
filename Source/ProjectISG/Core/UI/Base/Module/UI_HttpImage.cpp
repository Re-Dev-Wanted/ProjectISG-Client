#include "UI_HttpImage.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/Image.h"
#include "Styling/SlateBrush.h"
#include "Interfaces/IHttpResponse.h"

#include "ProjectISG/Utils/ApiUtil.h"

void UUI_HttpImage::SetImagePath(const FString& ImageUrl)
{
	if (ImageApiResponse.bIsLoading)
	{
		return;
	}

	FApiRequest Request;
	Request.Path = TEXT("/diary/render_image/") + ImageUrl;
	Request.Callback.BindLambda([this](FHttpRequestPtr Req,
	                                   FHttpResponsePtr Res,
	                                   const bool IsSuccess)
	{
		if (!IsSuccess || !Res.IsValid())
		{
			return;
		}

		const TArray<uint8>& ImageData = Res->GetContent();

		UTexture2D* Texture = LoadTextureFromImageData(ImageData);

		if (!Texture)
		{
			return;
		}

		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);
		Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());

		HttpImage->SetBrush(Brush);
	});

	FApiUtil::GetMainAPI()->GetApi(this, Request, ImageApiResponse);
}

TObjectPtr<UTexture2D> UUI_HttpImage::LoadTextureFromImageData(
	const TArray<uint8>& ImageData)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<
		IImageWrapperModule>(FName("ImageWrapper"));
	const EImageFormat Format = ImageWrapperModule.DetectImageFormat(
		ImageData.GetData(), ImageData.Num());

	if (Format == EImageFormat::Invalid)
	{
		return nullptr;
	}

	const TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.
		CreateImageWrapper(Format);

	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(
		ImageData.GetData(), ImageData.Num()))
	{
		TArray<uint8> RawData;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
		{
			UTexture2D* Texture = UTexture2D::CreateTransient(
				ImageWrapper->GetWidth(),
				ImageWrapper->GetHeight(),
				PF_B8G8R8A8);

			if (!Texture)
			{
				return nullptr;
			}

			void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.
				Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
			Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
			Texture->UpdateResource();

			return Texture;
		}
	}

	return nullptr;
}
