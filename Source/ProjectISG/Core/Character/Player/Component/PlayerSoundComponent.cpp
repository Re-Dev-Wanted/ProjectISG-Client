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

void UPlayerSoundComponent::StopTTSSound()
{
	TTSAudioComponent->Stop();
}

void UPlayerSoundComponent::PlayBGMSound(USoundWave* TargetBGM)
{
	if (!BGMAudioComponent)
	{
		BGMAudioComponent = NewObject<UAudioComponent>(this);
		BGMAudioComponent->bAutoActivate = false;
		BGMAudioComponent->bAutoDestroy = false;
		BGMAudioComponent->RegisterComponent();
		BGMAudioComponent->AttachToComponent(GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	}

	BGMAudioComponent->Stop();
	if (TargetBGM)
	{
		BGMAudioComponent->SetSound(TargetBGM);
		BGMAudioComponent->Play();
	}
}

void UPlayerSoundComponent::StopBGMSound()
{
	BGMAudioComponent->Stop();
}
