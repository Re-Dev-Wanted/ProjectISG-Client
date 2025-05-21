#include "UIC_QuestRequiredTextWidget.h"

#include "UIV_QuestRequiredTextWidget.h"
#include "Components/TextBlock.h"

void UUIC_QuestRequiredTextWidget::InitializeData(
	const FString& QuestRequireDescription,
	const FString& QuestRequireStatus) const
{
	const UUIV_QuestRequiredTextWidget* QuestRequiredTextWidgetView = Cast<
		UUIV_QuestRequiredTextWidget>(GetView());

	QuestRequiredTextWidgetView->GetRequireDescription()->
	                             SetText(
		                             FText::FromString(
			                             QuestRequireDescription));
	QuestRequiredTextWidgetView->GetRequireStatus()->SetText(
		FText::FromString(QuestRequireStatus));
}
