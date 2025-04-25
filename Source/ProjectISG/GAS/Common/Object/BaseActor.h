#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

class UISGAbilitySystemComponent;
class UISGAbilitySystemInitializeData;

UCLASS()
class PROJECTISG_API ABaseActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseActor();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnAttackAction(AActor* Causer) {}
	virtual void OnInteractAction(AActor* Causer) {}
	virtual void OnTouchAction(AActor* Causer) {}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UISGAbilitySystemInitializeData> InitialData;

	UPROPERTY(EditDefaultsOnly)
	UISGAbilitySystemComponent* AbilitySystemComponent;
};
