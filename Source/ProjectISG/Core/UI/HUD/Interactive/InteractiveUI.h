#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveUI.generated.h"

class UInteractiveItemUI;
class UVerticalBox;

UCLASS()
class PROJECTISG_API UInteractiveUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractive(bool Visible);
	
	void SetInteractive(const FString& Key, const FString& Text);

	void AddInteractive(const FString& Key, const FString& Text);

	void SetPlacementIndicator(bool Visible, bool Rotate = true);
	
	void ClearItems();

	bool IsInteractiveHidden();

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> Vertical_Root;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalView_Placement;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveItemUI> ExecuteInteractUI;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInteractiveItemUI> RightRotateUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteractiveItemUI> ItemFactory;
};
