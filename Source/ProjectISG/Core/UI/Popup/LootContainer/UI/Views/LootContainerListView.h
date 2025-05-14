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
	void Construct(TSubclassOf<UInventorySlot> InventorySlotClass, int32 SlotCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;
};
