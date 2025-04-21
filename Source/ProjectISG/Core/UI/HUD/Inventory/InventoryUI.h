#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "InventoryUI.generated.h"

class UInventoryList;

UCLASS()
class PROJECTISG_API UInventoryUI : public UUserWidget
{
public:
	GENERATED_BODY()

	GETTER(TObjectPtr<UInventoryList>, InventoryList)
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;
};
