﻿#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ItemInfo.generated.h"

class UImage;
class UMultiLineEditableTextBox;
class UTextBlock;
class UOverlay;

UCLASS()
class PROJECTISG_API UItemInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	// 두개 다 필요로 해서 둘 다 가져오게끔 처리
	void ShowItemData(const FItemMetaInfo& ItemMetaInfo);
	void RemoveItemWidget();

protected:
	virtual void
	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	bool IsActive;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> ItemPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDisplayName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemRank;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCategory;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableTextBox> ItemDescription;
};
