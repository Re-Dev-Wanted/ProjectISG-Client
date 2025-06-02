#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlayerHandSlotComponent.generated.h"

class AMainPlayerCharacter;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerHandSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerHandSlotComponent();

	GETTER(TObjectPtr<class ABaseActor>, HeldItem)
	GETTER(uint16, ItemId)

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializePlayerHandSlot();

	FString GetItemUsingType();

	bool IsHousingHandItem();

	void ClearHand();

	// 특정 UI 노출 및 특정 씬 진행 시 사용하는 값으로, 필요에 따라 사용하면 좋다.
	void ToggleShowItem(const bool IsShow);

protected:
	virtual void InitializeComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY()
	TObjectPtr<AMainPlayerCharacter> Player;

	UPROPERTY(Replicated)
	TObjectPtr<ABaseActor> HeldItem = nullptr;

	UPROPERTY(Replicated, EditAnywhere)
	uint16 ItemId = 0;

private:
	UFUNCTION()
	void OnInventoryUpdated();

	UFUNCTION(Server, Reliable)
	void Server_ChangeItem(uint16 ChangeItemId);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeItem(uint16 ChangeItemId,
	                            ABaseActor* ChangeHeldItem);
};
