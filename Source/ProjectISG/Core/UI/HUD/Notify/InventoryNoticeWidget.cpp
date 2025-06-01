#include "InventoryNoticeWidget.h"

#include "InventoryNoticeWidgetItem.h"
#include "Components/VerticalBox.h"

void UInventoryNoticeWidget::OnNotify(TSoftObjectPtr<UTexture2D> Image, FString Name)
{
	UInventoryNoticeWidgetItem* Item = CreateWidget<UInventoryNoticeWidgetItem>(this, ItemFactory);
	Item->OnNotify(Image, Name);

	VerticalBox->InsertChildAt(0, Item);
	VerticalBox->InvalidateLayoutAndVolatility();
}
