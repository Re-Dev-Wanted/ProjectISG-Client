#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemListGroup.generated.h"

class UItemInfoWidget;
class UHorizontalBox;

UCLASS()
class PROJECTISG_API UItemListGroup : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> ItemGroupBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UItemInfoWidget> WidgetFactory; 

	UPROPERTY(EditDefaultsOnly)
	int8 DebugCount = 5;
};
