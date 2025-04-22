#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveUI.generated.h"

class UTextBlock;

UCLASS()
class PROJECTISG_API UInteractiveUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractive(const FString& Key, const FString& Text) const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;
};
