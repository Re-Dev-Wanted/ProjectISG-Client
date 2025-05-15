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
	UE_DEPRECATED("5.5", "캐싱으로 인한 위험한 코드로, 가능하면 사용하지 않는 것을 권장")
	GETTER(uint16, ItemId)

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializePlayerHandSlot();

	UFUNCTION()
	void OnChange(uint16 ItemId);

	FString GetItemUsingType();

	bool IsHousingHandItem();

	void ClearHand();

protected:
	virtual void InitializeComponent() override;

	UPROPERTY()
	TObjectPtr<AMainPlayerCharacter> Player;

	UPROPERTY(Replicated, EditAnywhere)
	uint16 ItemId = 0;

	UPROPERTY(Replicated)
	TObjectPtr<class ABaseActor> HeldItem = nullptr;

private:
	UFUNCTION()
	void OnChangeHandItemSlot();

	UFUNCTION(Server, Reliable)
	void Server_ChangeItemId(uint16 ChangeItemId);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeItemId(uint16 ChangeItemId,
	                            class ABaseActor* ChangeHeldItem);
};
