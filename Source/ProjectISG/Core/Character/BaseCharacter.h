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

	// attribute를 초기화 하기위한 GameplayEffect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Options|GAS")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	void InitializePrimaryAttributes();

	//void AddCharacterAbilities();

	virtual void InitializeAbilitySystem()
	{
	};

	// UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
	// 	Category = "Options|GAS")
	// TArray<TSubclassOf<class UGameplayAbility>> StartupAbilities;
};
