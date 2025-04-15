#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"

class UISGAbilitySystemComponent;

UCLASS()
class PROJECTISG_API AMainPlayerState : public APlayerState,
                                        public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;
};
