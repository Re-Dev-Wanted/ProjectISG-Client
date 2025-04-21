#pragma once

#include "CoreMinimal.h"
#include "Base/MainPlayerComponent.h"
#include "PlayerInventoryComponent.generated.h"

struct FInputActionValue;

class UInputAction;
class UEnhancedInputComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerInventoryComponent : public UMainPlayerComponent
{
	GENERATED_BODY()

public:
	UPlayerInventoryComponent();

	void Initialize();

	bool RemoveItemCurrentSlotIndex(const int32 Count);

protected:
	virtual void BeginPlay() override;

private:
	bool IsOpenedInventory = false;
	int CurrentSlotIndex = 0;
	uint8 MaxMainSlotIndex = 8;

	virtual void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ToggleInventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SelectHotSlotInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveToHotSlotInputAction;

	void ToggleInventory();

	void SelectHotSlot(const FInputActionValue& Value);
	void MoveHotSlot(const FInputActionValue& Value);

	void ChangeCurrentSlotIndex(const uint8 NewIndex);

	UFUNCTION()
	void UpdatePlayerInventoryUI();
};
