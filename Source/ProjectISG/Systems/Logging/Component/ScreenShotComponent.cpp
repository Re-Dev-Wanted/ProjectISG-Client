#include "ScreenShotComponent.h"

#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

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

void UScreenShotComponent::SaveCaptureFrameImage(
	const FOnCaptureFrameNotified& OnCaptureFrameNotified)
{
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
	// GameThread_GetRenderTargetResource 는 이름 그대로 메인 스레드인
	// 게임 Thread에서 진행되기 때문에 다른 스레드에서 실행시키려고 한다면
	// 에러가 발생하기 때문에 무조건 메인 스레드에서만 실행시킬 수 있다.
	FTextureRenderTargetResource* RTResource = SceneCapture->TextureTarget->
		GameThread_GetRenderTargetResource();
	// 픽셀 읽기
	TArray<FColor> RawPixels;
	RTResource->ReadPixels(RawPixels);

	Async(EAsyncExecution::Thread, [this, RawPixels, OnCaptureFrameNotified]()
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
			                             TEXT("/screenshot/") +
			                             FDateTime::Now().ToString() +
			                             TEXT("-screenshot.png")));

		OnCaptureFrameNotified.Execute(CompressedData);
	});
}
