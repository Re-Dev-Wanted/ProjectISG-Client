#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerState.generated.h"

class UISGAttributeSet;
class UInventoryComponent;
class UISGAbilitySystemComponent;

UCLASS()
class PROJECTISG_API AMainPlayerState : public APlayerState,
                                        public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UISGAttributeSet* GetAttributeSet() const { return AttributeSet; }
	GETTER(TObjectPtr<UInventoryComponent>, InventoryComponent)

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UISGAttributeSet> AttributeSet;

private:
	void InitializeData();

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
