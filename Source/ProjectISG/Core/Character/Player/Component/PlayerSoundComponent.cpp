#include "PlayerSoundComponent.h"
#include "Components/AudioComponent.h"


UPlayerSoundComponent::UPlayerSoundComponent()
{
}

void UPlayerSoundComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerSoundComponent::PlayTTSSound(USoundWave* TargetTTS)
{
	if (!TTSAudioComponent)
	{
		// 처음 한 번만 생성
		TTSAudioComponent = NewObject<UAudioComponent>(this);
		TTSAudioComponent->bAutoActivate = false;
		TTSAudioComponent->bAutoDestroy = false;
		TTSAudioComponent->RegisterComponent();
		TTSAudioComponent->AttachToComponent(GetOwner()->GetRootComponent(),
		                                     FAttachmentTransformRules::KeepRelativeTransform);
	}

	TTSAudioComponent->Stop();

	if (TargetTTS)
	{
		TTSAudioComponent->SetSound(TargetTTS);
		TTSAudioComponent->Play();
	}
}
