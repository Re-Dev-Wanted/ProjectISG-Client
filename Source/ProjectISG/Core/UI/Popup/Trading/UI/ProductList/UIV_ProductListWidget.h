#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "UIV_ProductListWidget.generated.h"

class UButton;
class UUIV_ProductInfoWidget;

UENUM()
enum class ETradingTab : uint8
{
	BUY,
	SELL
};

UCLASS()
class PROJECTISG_API UUIV_ProductListWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UListView*, ProductList)
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTradingTabButton* BuyTabButton;

	UPROPERTY(meta = (BindWidget))
	class UTradingTabButton* SellTabButton;
	
	UPROPERTY(meta = (BindWidget))
	class UListView* ProductList;
};
