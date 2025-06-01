#include "GC_Seeding.h"

#include "Components/AudioComponent.h"

bool AGC_Seeding::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	SpawnSound();

	SetActorLocation(MyTarget->GetActorLocation());

	AudioComponent->Stop();
	AudioComponent->Play();

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

void AGC_Seeding::SpawnSound()
{
	if (!SeedingSound)
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

		AudioComponent->SetSound(SeedingSound);
		AudioComponent->AttachToComponent(GetRootComponent(),
										  FAttachmentTransformRules::KeepRelativeTransform);
	}
}
