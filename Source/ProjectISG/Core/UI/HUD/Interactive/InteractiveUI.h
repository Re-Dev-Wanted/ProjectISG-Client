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
	void SetInteractive(const FString& Key, const FString& Text) const;

	void AddInteractive(const FString& Key, const FString& Text) const;
	
	void ClearItems();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalView;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInteractiveItemUI> ItemFactory;
};
