#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GC_Seeding.generated.h"

UCLASS()
class PROJECTISG_API AGC_Seeding : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget,
										  const FGameplayCueParameters&
										  Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> SeedingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

	void SpawnSound();
};
