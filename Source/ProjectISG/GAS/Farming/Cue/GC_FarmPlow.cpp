#include "GC_FarmPlow.h"
#include "Components/AudioComponent.h"


AGC_FarmPlow::AGC_FarmPlow()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AGC_FarmPlow::OnExecute_Implementation(AActor* MyTarget,
                                            const FGameplayCueParameters&
                                            Parameters)
{
	SpawnSound();

	SetActorLocation(MyTarget->GetActorLocation());

	AudioComponent->Stop();
	AudioComponent->Play();

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

void AGC_FarmPlow::SpawnSound()
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
