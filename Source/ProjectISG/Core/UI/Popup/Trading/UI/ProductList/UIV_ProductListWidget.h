#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Popup/Trading/UI/UIC_TradingUI.h"
#include "UIV_ProductListWidget.generated.h"

enum class ETradingState : uint8;
class UButton;
class UUIV_ProductInfoWidget;

UCLASS()
class PROJECTISG_API UUIV_ProductListWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UTradingTabButton*, BuyTabButton)
	GETTER(class UTradingTabButton*, SellTabButton)
	GETTER(class UListView*, ProductList)
	
	void SetUpdateUI(ETradingState State);
	
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
