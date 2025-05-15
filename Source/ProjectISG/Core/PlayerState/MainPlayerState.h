#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerState.generated.h"

class ATimeManager;
class AGridManager;
class ULootContainerSubsystem;
class UInventoryComponent;
class UISGAbilitySystemComponent;

UCLASS()
class PROJECTISG_API AMainPlayerState
	: public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerState();
	void InitializeData();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UISGAttributeSet* GetAttributeSet() const { return AttributeSet; }

	GETTER(TObjectPtr<UInventoryComponent>, InventoryComponent)
	GETTER(TObjectPtr<AGridManager>, GridManager)
	GETTER(TObjectPtr<ATimeManager>, TimeManager);
	GETTER(TObjectPtr<ULootContainerSubsystem>, LootContainerComponent)
	GETTER_SETTER(int32, Gold);

	bool CanBuyProduct(int32 ProductPrice);

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UISGAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<AGridManager> GridManager;

	UPROPERTY(VisibleAnywhere, Replicated)
	TObjectPtr<ULootContainerSubsystem> LootContainerComponent;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<ATimeManager> TimeManager;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 Gold = 10000;
};
