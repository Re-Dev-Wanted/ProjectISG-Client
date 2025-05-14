#include "UIC_CurrentQuestWidget.h"

#include "UIV_CurrentQuestWidget.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"

void UUIC_CurrentQuestWidget::StartCurrentQuestAnimation()
{
	const UUIV_CurrentQuestWidget* CurrentQuestWidget = Cast<
		UUIV_CurrentQuestWidget>(
		GetView());

	GetView()->PlayAnimation(CurrentQuestWidget->GetQuestAppearAnimation());
}
