#include "ItemListGroup.h"

#include "Components/HorizontalBox.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Widget/ItemInfoWidget.h"

void UItemListGroup::NativePreConstruct()
{
	Super::NativePreConstruct();

	ItemGroupBox->ClearChildren();

	for (int8 i = 0; i < DebugCount; i++)
	{
		UItemInfoWidget* Widget = CreateWidget<UItemInfoWidget>(this, WidgetFactory);
		ItemGroupBox->AddChildToHorizontalBox(Widget);
	}
}
