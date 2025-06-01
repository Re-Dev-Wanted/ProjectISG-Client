#include "UIC_SceneDialogueWidget.h"

#include "UIV_SceneDialogueWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerSoundComponent.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_SceneDialogueWidget::StartSceneDialogue(
	const FString& SceneId, const uint8 SceneIndex)
{
	FQuestSceneCutData SceneCutData =
		UQuestStoryManager::GetQuestSceneCutById(SceneId);

	UUIV_SceneDialogueWidget* SceneDialogueWidgetView = Cast<
		UUIV_SceneDialogueWidget>(GetView());

	SceneDialogueWidgetView->GetDialogueText()->SetText(
		SceneCutData.GetSceneCutList()[SceneIndex].GetSceneComment());

	if (SceneIndex <= SceneCutData.GetSceneCommentTTS().Num())
	{
		Cast<AMainPlayerCharacter>(GetView()->GetOwningPlayerPawn())->
			GetPlayerSoundComponent()->PlayTTSSound(
				SceneCutData.GetSceneCommentTTS()[SceneIndex]);
	}

	SceneDialogueWidgetView->PlayAnimation(
		SceneDialogueWidgetView->GetDialogueAnimation());
}
