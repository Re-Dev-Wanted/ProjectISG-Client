// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_TradingUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_TradingUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UUIV_ProductListWidget*, ProductList)
	GETTER(class UInventoryList*, InventoryList)
	GETTER(class UTextBlock*, GoldText)
	SETTER(bool, OpenFlag)
	GETTER(class UItemInfo*, ItemInfoTooltip)

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry,
	                          const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

	virtual void NativeConstruct() override;

	virtual void
	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ProductListScroll;

	UPROPERTY(meta = (BindWidget))
	class UInventoryList* InventoryList;

	UPROPERTY(meta = (BindWidget))
	class UUIV_ProductListWidget* ProductList;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UItemInfo* ItemInfoTooltip;

	bool OpenFlag = false;
};
