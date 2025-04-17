#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainHUD.generated.h"

class UInventoryList;

UCLASS()
class PROJECTISG_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)

	void InitializeHUD();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;
};
