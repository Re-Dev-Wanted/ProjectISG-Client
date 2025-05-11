#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MainHUD.generated.h"

class UInteractiveUI;
class UInventoryList;
class UUIV_Time;

UCLASS()
class PROJECTISG_API UUIV_MainHUD : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)
	GETTER(TObjectPtr<UInteractiveUI>, InteractiveUI)
	GETTER(TObjectPtr<UUIV_Time>, TimeUI)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveUI> InteractiveUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_Time> TimeUI;
};
