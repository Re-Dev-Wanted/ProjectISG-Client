﻿#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSoundComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerSoundComponent();

	void PlayTTSSound(USoundWave* TargetTTS);

	void StopTTSSound();
	
	void PlayBGMSound(USoundWave* TargetBGM);
	
	void StopBGMSound();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> TTSAudioComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> BGMAudioComponent;
};
