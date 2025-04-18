#include "ScreenShotComponent.h"

#include "ImageUtils.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"


UScreenShotComponent::UScreenShotComponent()
{
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(
		TEXT("SceneCaptureComponent"));

	// 사람 눈에 보이는 것을 기준으로 캡쳐하도록 설정해주는 역할을 수행함.
	// 분석용이 아닌 유저 감상용이라면 Final을 쓰는 것이 좋아보인다.
	SceneCapture->CaptureSource = SCS_FinalColorLDR;
}

void UScreenShotComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UScreenShotComponent::SaveCaptureFrameImage()
{
	FDateTime TestDate = FDateTime::Now();
	SceneCapture->TextureTarget =
		UKismetRenderingLibrary::CreateRenderTarget2D(
			GetWorld(),
			1920,
			1080,
			RTF_RGBA16f,
			FLinearColor::White,
			false,
			false
		);
	SceneCapture->CaptureScene();
	SceneCapture->bCaptureEveryFrame = false;
	// RenderTarget에서 리소스 얻기
	FTextureRenderTargetResource* RTResource = SceneCapture->TextureTarget->
		GameThread_GetRenderTargetResource();
	// 픽셀 읽기
	TArray<FColor> RawPixels;
	RTResource->ReadPixels(RawPixels);

	Async(EAsyncExecution::Thread, [this, RawPixels, TestDate]()
	{
		// 이미지 래퍼 모듈
		IImageWrapperModule& ImageWrapperModule =
			FModuleManager::LoadModuleChecked<
				IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.
			CreateImageWrapper(EImageFormat::PNG);

		// 압축
		ImageWrapper->SetRaw(RawPixels.GetData(),
		                     RawPixels.Num() * sizeof(FColor),
		                     SceneCapture->TextureTarget->SizeX,
		                     SceneCapture->TextureTarget->SizeY,
		                     ERGBFormat::BGRA, 8);
		const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed();

		// 저장
		FFileHelper::SaveArrayToFile(CompressedData,
		                             *(FPaths::ProjectSavedDir() +
			                             TEXT("/test.png")));

		UE_LOG(LogTemp, Display, TEXT("Async Test Result: %s"),
		       *(FDateTime::Now() - TestDate).ToString());
	});
}

void UScreenShotComponent::SaveCaptureFrameImageSync()
{
	FDateTime TestDate = FDateTime::Now();
	SceneCapture->TextureTarget =
		UKismetRenderingLibrary::CreateRenderTarget2D(
			GetWorld(),
			1920,
			1080,
			RTF_RGBA16f,
			FLinearColor::White,
			false,
			false
		);
	SceneCapture->CaptureScene();
	SceneCapture->bCaptureEveryFrame = false;
	// RenderTarget에서 리소스 얻기
	FTextureRenderTargetResource* RTResource = SceneCapture->TextureTarget->
		GameThread_GetRenderTargetResource();
	// 픽셀 읽기
	TArray<FColor> RawPixels;
	RTResource->ReadPixels(RawPixels);

	// 이미지 래퍼 모듈
	IImageWrapperModule& ImageWrapperModule =
		FModuleManager::LoadModuleChecked<
			IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.
		CreateImageWrapper(EImageFormat::PNG);

	// 압축
	ImageWrapper->SetRaw(RawPixels.GetData(),
	                     RawPixels.Num() * sizeof(FColor),
	                     SceneCapture->TextureTarget->SizeX,
	                     SceneCapture->TextureTarget->SizeY,
	                     ERGBFormat::BGRA, 8);
	const TArray64<uint8>& CompressedData = ImageWrapper->GetCompressed();

	// 저장
	FFileHelper::SaveArrayToFile(CompressedData,
	                             *(FPaths::ProjectSavedDir() +
		                             TEXT("/test.png")));

	UE_LOG(LogTemp, Display, TEXT("Sync Test Result: %s"),
	       *(FDateTime::Now() - TestDate).ToString());
}

TArray64<uint8> UScreenShotComponent::CaptureAndGetFile()
{
	// RenderTarget이 애초부터 없으면 장애 상황이다.
	FRenderTarget* RenderTargetResource = SceneCapture->TextureTarget->
		GameThread_GetRenderTargetResource();

	TArray<FColor> OutBMP;
	RenderTargetResource->ReadPixels(OutBMP);

	TArray64<uint8> PNGData;
	FImageUtils::PNGCompressImageArray(SceneCapture->TextureTarget->SizeX,
	                                   SceneCapture->TextureTarget->SizeY,
	                                   OutBMP, PNGData);

	return PNGData;
}
