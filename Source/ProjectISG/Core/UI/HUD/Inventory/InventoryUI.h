#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

class UInventorySlot;
class UGridPanel;

UCLASS()
class PROJECTISG_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, Category = "Options|UI|InventoryList",
		meta = (AllowPrivateAccess))
	uint8 ListRow = 8;

	UPROPERTY(EditAnywhere, Category = "Options|UI|InventoryList",
		meta = (AllowPrivateAccess))
	uint8 ListColumn = 6;

	UPROPERTY(EditAnywhere, Category = "Options|UI|InventoryList",
		meta = (AllowPrivateAccess))
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> InventoryList;

	void InitializeItemData();
};
