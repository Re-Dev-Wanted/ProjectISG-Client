#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MaterialsView.generated.h"

class UImage;
struct FCraftingMaterialUIModel;
class UMaterialInfoWidget;
class UScrollBox;
class UTextBlock;

UCLASS()
class PROJECTISG_API UMaterialsView : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnUpdateUI(const FString& ItemName,
	                const TArray<FCraftingMaterialUIModel>& Materials,
	                const TMap<uint16, uint16>& OwningCounts);

	void SetDescription(const FText& Desc);

	void SetImage(TSoftObjectPtr<UTexture2D> Thumbnail);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMaterialInfoWidget> WidgetFactory;

	UPROPERTY(EditDefaultsOnly)
	uint8 DebugCount = 10;
};
