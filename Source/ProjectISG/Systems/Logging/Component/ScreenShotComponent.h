#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenShotComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnCaptureFrameNotified,
                          TArray64<uint8> ImageBinaryData);

class USpringArmComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UScreenShotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScreenShotComponent();

	void SaveCaptureFrameImage(const UObject* Object,
	                           const FOnCaptureFrameNotified&
		                           OnCaptureFrameNotified =
		                           FOnCaptureFrameNotified());

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneCaptureComponent2D> SceneCapture;
};
