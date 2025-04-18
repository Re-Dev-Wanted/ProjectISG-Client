#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenShotComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UScreenShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScreenShotComponent();

	TArray64<uint8> CaptureAndGetFile();

	void SaveCaptureFrameImage();
	void SaveCaptureFrameImageSync();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;
};
