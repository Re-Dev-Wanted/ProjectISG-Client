#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MainHUD.generated.h"

class UInteractiveUI;
class UInventoryList;

UCLASS()
class PROJECTISG_API UUIV_MainHUD : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)
	GETTER(TObjectPtr<UInteractiveUI>, InteractiveUI)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveUI> InteractiveUI;
};
