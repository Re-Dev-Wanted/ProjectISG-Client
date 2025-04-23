#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_InventoryUI.generated.h"

class UInventoryList;

UCLASS()
class PROJECTISG_API UUIV_InventoryUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, InventoryList)
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;
};
