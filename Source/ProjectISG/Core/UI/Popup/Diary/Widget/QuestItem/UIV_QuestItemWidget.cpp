#include "UIV_QuestItemWidget.h"

#include "QuestItemWidgetObject.h"
#include "UIC_QuestItemWidget.h"

void UUIV_QuestItemWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	const UQuestItemWidgetObject* QuestItemWidgetObject = Cast<
		UQuestItemWidgetObject>(ListItemObject);

	if (!QuestItemWidgetObject)
	{
		return;
	}

	Cast<UUIC_QuestItemWidget>(GetController())->InitializeData(
		QuestItemWidgetObject->QuestId);
}
