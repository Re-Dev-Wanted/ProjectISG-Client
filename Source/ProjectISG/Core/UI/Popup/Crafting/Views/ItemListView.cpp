#include "ItemListView.h"

#include "ItemListGroup.h"
#include "Components/ScrollBox.h"

void UItemListView::NativePreConstruct()
{
	Super::NativePreConstruct();

	ScrollBox->ClearChildren();

	for (uint8 i = 0; i < DebugCount; i++)
	{
		UItemListGroup* Group = CreateWidget<UItemListGroup>(this, ItemFactory);
		ScrollBox->AddChild(Group);
	}
}
