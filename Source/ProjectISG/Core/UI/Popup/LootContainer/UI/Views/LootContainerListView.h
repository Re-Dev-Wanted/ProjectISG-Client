#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootContainerListView.generated.h"

class UWrapBox;
class UInventorySlot;

UCLASS()
class PROJECTISG_API ULootContainerListView : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnInitialize(int32 SlotCount);

protected:
	UPROPERTY(EditAnywhere, Category = "Options|UI",
		meta = (AllowPrivateAccess))
	TSubclassOf<UInventorySlot> InventorySlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;
};
