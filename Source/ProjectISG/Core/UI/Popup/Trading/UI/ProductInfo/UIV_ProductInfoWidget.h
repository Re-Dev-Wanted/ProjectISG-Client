#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ProductInfoWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIV_ProductInfoWidget : public UBaseUIView, public 
IUserObjectListEntry
{
	GENERATED_BODY()

public:
	GETTER(class UButton*, ProductSelectButton);
	GETTER(class UProductThumbnailWidget*, ProductThumbnail);
	GETTER(class UTextBlock*, ProductName);
	GETTER(class UTextBlock*, ProductPrice);

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ProductSelectButton;

	UPROPERTY(meta = (BindWidget))
	class UProductThumbnailWidget* ProductThumbnail;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProductName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProductPrice;
};
