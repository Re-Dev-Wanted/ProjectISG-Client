#include "MaterialsView.h"

#include "Components/ScrollBox.h"
#include "ProjectISG/Core/UI/Popup/Crafting/Widget/MaterialInfoWidget.h"

void UMaterialsView::NativePreConstruct()
{
	Super::NativePreConstruct();

	ScrollBox->ClearChildren();

	for (int8 i = 0; i < DebugCount; i++)
	{
		UMaterialInfoWidget* Widget = CreateWidget<UMaterialInfoWidget>(this, WidgetFactory);
		ScrollBox->AddChild(Widget);
	}
}
