#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"

#include "GC_FarmWatering.generated.h"

class UNiagaraComponent;

UCLASS()
class PROJECTISG_API AGC_FarmWatering : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGC_FarmWatering();

protected:
	virtual void BeginPlay() override;

	virtual bool OnExecute_Implementation(AActor* MyTarget,
	                                      const FGameplayCueParameters&
	                                      Parameters) override;

	virtual bool OnRemove_Implementation(AActor* MyTarget,
	                                     const FGameplayCueParameters&
	                                     Parameters) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraComponent> WateringEffect;
};
