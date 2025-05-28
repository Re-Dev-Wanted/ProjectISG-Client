#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "LootContainerListView.generated.h"

class ULootContainerItemSlot;
class UWrapBox;

UCLASS()
class PROJECTISG_API ULootContainerListView : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetContainerInfo(const TSubclassOf<ULootContainerItemSlot>& InventorySlotClass, TArray<FItemMetaInfo> Items, TScriptInterface<IItemHandler> Handler);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;
};
