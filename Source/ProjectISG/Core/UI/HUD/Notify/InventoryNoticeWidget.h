#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryNoticeWidget.generated.h"

class UInventoryNoticeWidgetItem;
class UVerticalBox;
class UTexture2D;

UCLASS()
class PROJECTISG_API UInventoryNoticeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnNotify(TSoftObjectPtr<UTexture2D> Image, FString Name);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(EditAnywhere, Category = Item)
	TSubclassOf<UInventoryNoticeWidgetItem> ItemFactory;
};
