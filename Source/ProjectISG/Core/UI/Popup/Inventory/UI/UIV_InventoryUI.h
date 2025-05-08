#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_InventoryUI.generated.h"

class UItemInfo;
class UInventoryList;

UCLASS()
class PROJECTISG_API UUIV_InventoryUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, InventoryList)
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)
	GETTER(TObjectPtr<UItemInfo>, ItemInfoTooltip)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemInfo> ItemInfoTooltip;
};
