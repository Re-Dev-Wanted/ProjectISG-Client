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
	void InitializeData();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UISGAttributeSet* GetAttributeSet() const { return AttributeSet; }
	GETTER(TObjectPtr<UInventoryComponent>, InventoryComponent)

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UISGAttributeSet> AttributeSet;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
