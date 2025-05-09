#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class ULevelSequencePlayer;

UCLASS()
class PROJECTISG_API ABaseCharacter : public ACharacter,
                                      public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void PlayCinematic(ULevelSequencePlayer* Cinematic);

	UFUNCTION(Client, Reliable)
	void Client_PlayCinematic(ULevelSequencePlayer* Cinematic);

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

	virtual void InitializeAbilitySystem()
	{
	}
};
