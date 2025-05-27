#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Module/UI_EscapeButton.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_TradingUI.generated.h"

UCLASS()
class PROJECTISG_API UUIV_TradingUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(class UTextBlock*, OwnedGoldText)
	GETTER(class UUIV_ProductListWidget*, ItemListView)
	GETTER(class UProductDetailView*, ProductDetailView)
	SETTER(bool, OpenFlag)

	class UButton* GetCloseButton() const
	{
		return CloseButton->GetButton();
	}

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry,
	                          const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

	virtual void NativeConstruct() override;

	virtual void
	NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OwnedGoldText;
	
	UPROPERTY(meta = (BindWidget))
	class UUI_EscapeButton* CloseButton;
	
	UPROPERTY(meta = (BindWidget))
	class UProductDetailView* ProductDetailView;
	
	UPROPERTY(meta = (BindWidget))
	class UUIV_ProductListWidget* ItemListView;

	bool OpenFlag = false;
};
