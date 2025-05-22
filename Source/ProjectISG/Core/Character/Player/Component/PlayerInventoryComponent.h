#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlayerInventoryComponent.generated.h"

struct FInputActionValue;

class UInputAction;
class UEnhancedInputComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInventoryComponent();

	bool RemoveItemCurrentSlotIndex(const int32 Count);

	void InitializePlayerInventory();

	GETTER(int, CurrentSlotIndex);

	void UpdateInventorySlotItemData();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	bool IsOpenedInventory = false;

	UPROPERTY(Replicated, EditAnywhere)
	int CurrentSlotIndex = 0;

	uint8 MaxMainSlotIndex = 8;

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> OpenInventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SelectHotSlotInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveToHotSlotInputAction;

	void OpenInventory();

	void SelectHotSlot(const FInputActionValue& Value);
	void MoveHotSlot(const FInputActionValue& Value);

	void ChangeCurrentSlotIndex(const uint8 NewIndex);

	UFUNCTION()
	void UpdatePlayerInventoryUI();

	UFUNCTION(Reliable, Server)
	void Server_ChangeCurrentSlotIndex(int idx);
};
