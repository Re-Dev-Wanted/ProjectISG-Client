#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "BaseActor.generated.h"

class UISGAbilitySystemComponent;
class UISGAbilitySystemInitializeData;

UCLASS()
class PROJECTISG_API ABaseActor : public AActor, public 
IAbilitySystemInterface, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ABaseActor();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UISGAbilitySystemInitializeData> InitialData;

	UPROPERTY(EditDefaultsOnly)
	UISGAbilitySystemComponent* AbilitySystemComponent;
};
