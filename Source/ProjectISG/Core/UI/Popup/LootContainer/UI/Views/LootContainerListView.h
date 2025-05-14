#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootContainerListView.generated.h"

class ULootContainerItemSlot;
class UWrapBox;

UCLASS()
class PROJECTISG_API ULootContainerListView : public UUserWidget
{
	GENERATED_BODY()

public:
	void Construct(const TSubclassOf<ULootContainerItemSlot>& InventorySlotClass, int32 SlotCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;
};
