#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerState.generated.h"

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

	class UISGAttributeSet* GetAttributeSet() const { return AttributeSet; }

	GETTER(TObjectPtr<UInventoryComponent>, InventoryComponent)
	GETTER(TObjectPtr<class AGridManager>, GridManager)
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<class UISGAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<class AGridManager> GridManager;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
