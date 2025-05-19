#include "UIC_QuestListUI.h"

#include "UIV_QuestListUI.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_QuestListUI::AppearUI()
{
	Super::AppearUI();

	const UUIV_QuestListUI* QuestListUIView = Cast<UUIV_QuestListUI>(GetView());

	const ATimeManager* TimeManager = QuestListUIView->GetOwningPlayerState<
		AMainPlayerState>()->GetTimeManager();

	QuestListUIView->GetCurrentDateTime()->SetText(
		FText::FromString(TimeManager->GetDateDisplayText()));

	QuestListUIView->GetUserId()->SetText(
		FText::FromString(FSessionUtil::GetCurrentId(GetWorld())));
}
