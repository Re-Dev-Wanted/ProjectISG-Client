#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_MainHUD.generated.h"

class UQuestCompleteNotification;
class UInventoryNoticeWidget;
class UInventoryNoticeWidgetItem;
class UUIV_HUDAlertModalWidget;
class UUIV_CurrentQuestWidget;
class UUIV_AutoQuestDialogueWidget;
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
	GETTER(TObjectPtr<UUIV_AutoQuestDialogueWidget>, AutoQuestDialogueWidget)
	GETTER(TObjectPtr<UUIV_CurrentQuestWidget>, CurrentQuestWidget)
	GETTER(TObjectPtr<UUIV_HUDAlertModalWidget>, AlertModalWidget)
	GETTER(TObjectPtr<UInventoryNoticeWidget>, InventoryNoticeWidget)
	GETTER(TObjectPtr<UQuestCompleteNotification>, QuestCompleteNotification)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveUI> InteractiveUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_Time> TimeUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_AutoQuestDialogueWidget> AutoQuestDialogueWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_CurrentQuestWidget> CurrentQuestWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIV_HUDAlertModalWidget> AlertModalWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryNoticeWidget> InventoryNoticeWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQuestCompleteNotification> QuestCompleteNotification;
};
