#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListView.generated.h"

class UItemListGroup;
class UScrollBox;

UCLASS()
class PROJECTISG_API UItemListView : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemListGroup> ItemFactory;

	UPROPERTY(EditDefaultsOnly)
	int8 DebugCount = 10;
};
