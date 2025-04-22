#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainHUD.generated.h"

class UInteractiveUI;
class UInventoryList;

UCLASS()
class PROJECTISG_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UInventoryList>, MainSlotList)
	GETTER(TObjectPtr<UInteractiveUI>, InteractiveUI)

	void InitializeHUD();

	void ToggleInteractiveUI(const bool IsShow);
	void ToggleInteractiveUI(const FString& DisplayKey,
	                         const FString& DisplayText);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveUI> InteractiveUI;
};
