#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class UISGAttributeSet;
class UISGAbilitySystemInitializeData;
class UISGAbilitySystemComponent;

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
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Options|GAS")
	TObjectPtr<UISGAbilitySystemInitializeData> InitializeData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|GAS")
	TObjectPtr<UISGAttributeSet> AttributeSet;

	virtual void InitializeAbilitySystem()
	{
	};
};
