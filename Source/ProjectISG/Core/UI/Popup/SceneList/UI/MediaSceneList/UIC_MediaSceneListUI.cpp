#include "UIC_MediaSceneListUI.h"
#include "UIV_MediaSceneListUI.h"

#include "EnhancedInputComponent.h"
#include "MediaPlayer.h"
#include "UIM_MediaSceneListUI.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_MediaSceneListUI::AppearUI()
{
	Super::AppearUI();
	// 해당 타입이 무조건 Media 타입이라고 가정하고 한다.
	// 잘못된 타입일 시 그냥 터트리는게 맞음.
	UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());
	SceneListModel->SetCurrentSceneId(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetQuestManageComponent()->GetCurrentPlayingSceneId());

	FQuestSceneCutData MediaSceneData =
		UQuestStoryManager::GetQuestSceneCutById(
			SceneListModel->GetCurrentSceneId());

	MediaSceneData.GetSceneMedia().GetStartCinematic()->OnEndReached.
	               AddDynamic(this, &ThisClass::OnEndMediaScene);

	if (MediaSceneData.GetSceneMedia().GetStartCinematic()->OpenSource(
		MediaSceneData.GetSceneMedia().GetStartCinematicSource()))
	{
		MediaSceneData.GetSceneMedia().GetStartCinematic()->Rewind();
	}
}

void UUIC_MediaSceneListUI::OnEndMediaScene()
{
	OnEndMediaSceneNotified.Execute();
	OnEndMediaSceneNotified.Clear();
}

void UUIC_MediaSceneListUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnTriggerSkipSceneAction);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Completed, this,
	                           &ThisClass::OnEndSkipSceneAction);
}

void UUIC_MediaSceneListUI::OnTriggerSkipSceneAction()
{
	const UUIV_MediaSceneListUI* SceneListView = Cast<UUIV_MediaSceneListUI>(
		GetView());
	UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());

	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Visible);
	SceneListModel->SetCurrentLoadingPercent(
		SceneListModel->GetCurrentLoadingPercent() + 1);

	const float CurrentPercent = SceneListModel->
		GetCurrentLoadingPercent();

	SetSkipCircularPercent(
		CurrentPercent / SceneListModel->GetMaxLoadingPercent());

	if (SceneListModel->GetCurrentLoadingPercent() >= SceneListModel->
		GetMaxLoadingPercent())
	{
		OnEndMediaScene();
	}
}

void UUIC_MediaSceneListUI::OnEndSkipSceneAction()
{
	const UUIV_MediaSceneListUI* SceneListView = Cast<UUIV_MediaSceneListUI>(
		GetView());
	UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());

	SceneListModel->SetCurrentLoadingPercent(0);
	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Hidden);
}

void UUIC_MediaSceneListUI::SetSkipCircularPercent(const float Percent)
{
	const UUIV_MediaSceneListUI* SceneListView = Cast<UUIV_MediaSceneListUI>(
		GetView());

	SceneListView->GetCircularLoadingWidget()->GetDynamicMaterial()->
	               SetScalarParameterValue(FName("Percent"), Percent);
}
