#include "GC_Farming.h"
#include "Components/AudioComponent.h"

AGC_Farming::AGC_Farming()
{
}

bool AGC_Farming::OnExecute_Implementation(AActor* MyTarget,
                                           const FGameplayCueParameters&
                                           Parameters)
{
	SpawnSound();

	SetActorLocation(MyTarget->GetActorLocation());

	AudioComponent->Stop();
	AudioComponent->Play();

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

void AGC_Farming::SpawnSound()
{
	if (!FarmingSound)
	{
		return;
	}

	if (!AudioComponent)
	{
		// 처음 한 번만 생성
		AudioComponent = NewObject<UAudioComponent>(this);
		AudioComponent->bAutoActivate = false;
		AudioComponent->bAutoDestroy = false;
		AudioComponent->RegisterComponent();

		AudioComponent->SetSound(FarmingSound);
		AudioComponent->AttachToComponent(GetRootComponent(),
		                                  FAttachmentTransformRules::KeepRelativeTransform);
	}
}
