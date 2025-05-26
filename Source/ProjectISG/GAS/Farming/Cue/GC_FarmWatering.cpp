#include "GC_FarmWatering.h"
#include "NiagaraComponent.h"

AGC_FarmWatering::AGC_FarmWatering()
{
	WateringEffect = CreateDefaultSubobject<UNiagaraComponent>(
		"Watering Effect");
}

void AGC_FarmWatering::BeginPlay()
{
	Super::BeginPlay();
}

bool AGC_FarmWatering::OnExecute_Implementation(AActor* MyTarget,
                                                const FGameplayCueParameters&
                                                Parameters)
{
	UE_LOG(LogTemp, Display, TEXT("123"))

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

bool AGC_FarmWatering::OnRemove_Implementation(AActor* MyTarget,
                                               const FGameplayCueParameters&
                                               Parameters)
{
	UE_LOG(LogTemp, Display, TEXT("으아악"))

	return Super::OnRemove_Implementation(MyTarget, Parameters);
}
