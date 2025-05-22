#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECTISG_API ABaseCharacter : public ACharacter,
                                      public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Options|GAS")
	TObjectPtr<class UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Options|GAS")
	TObjectPtr<class UISGAbilitySystemInitializeData> InitializeData;

	UPROPERTY()
	TObjectPtr<class UISGAttributeSet> AttributeSet;

	virtual void InitializeAbilitySystem()
	{
	};
};
