// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ProductListWidget.generated.h"

class UUIV_ProductInfoWidget;
/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIV_ProductListWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UGridPanel*, ProductListGrid);
	GETTER(TSubclassOf<UUIV_ProductInfoWidget>, ProductInfoWidgetClass);
	GETTER(uint16, DebugProductCount);
	GETTER(uint16, ColumnValue);

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UGridPanel* ProductListGrid;

	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UUIV_ProductInfoWidget> ProductInfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint16 DebugProductCount;

	UPROPERTY(EditAnywhere, Category = "Options",
		meta = (AllowPrivateAccess = true))
	uint16 ColumnValue;
};
