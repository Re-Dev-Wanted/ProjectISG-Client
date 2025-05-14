#include "UIC_SceneListUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_SceneListUI.h"
#include "UIV_SceneListUI.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_SceneListUI::InitializeController(UBaseUIView* NewView,
                                            UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);
}

void UUIC_SceneListUI::AppearUI(const EUILayer Layer)
{
	Super::AppearUI(Layer);

	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

	// 현재 씬 아이디를 가져온다.
	SceneListModel->SetCurrentSceneId(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetQuestManageComponent()->GetCurrentPlayingSceneId());

	// 아이디를 가져오면 그 아이디 기반으로 데이터를 가져온다.
	FQuestSceneCutData CutData = UQuestStoryManager::GetQuestSceneCutById(
		SceneListModel->GetCurrentSceneId());

	SceneListModel->SetCurrentSceneIndex(0);
	SceneListModel->SetMaxSceneIndex(CutData.GetSceneImages().Num());

	GetWorld()->GetTimerManager().SetTimer(SceneCutChangeTimerHandle, this,
	                                       &ThisClass::MoveToNextScene, 4,
	                                       true, 0);
}

void UUIC_SceneListUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnTriggerSkipSceneAction);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Completed, this,
	                           &ThisClass::OnEndSkipSceneAction);
}

void UUIC_SceneListUI::OnTriggerSkipSceneAction()
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());
	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

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
		GetView()->GetOwningPlayer<AMainPlayerController>()->PopUI();
		// TODO: 추후 행동에 대한 Delegate를 받아도 상관없을 것 같다
	}

	GetWorld()->GetTimerManager().ClearTimer(SceneCutChangeTimerHandle);
}

void UUIC_SceneListUI::OnEndSkipSceneAction()
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());
	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

	SceneListModel->SetCurrentLoadingPercent(0);
	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Hidden);
}

void UUIC_SceneListUI::SetSkipCircularPercent(const float Percent)
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());

	SceneListView->GetCircularLoadingWidget()->GetDynamicMaterial()->
	               SetScalarParameterValue(FName("Percent"), Percent);
}

void UUIC_SceneListUI::MoveToNextScene()
{
	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());

	if (SceneListModel->GetCurrentSceneIndex() >= SceneListModel->
		GetMaxSceneIndex())
	{
		GetWorld()->GetTimerManager().ClearTimer(SceneCutChangeTimerHandle);
		GetView()->GetOwningPlayer<AMainPlayerController>()->PopUI();

		if (OnSceneListEndNotified.IsBound())
		{
			OnSceneListEndNotified.Execute();
		}

		return;
	}

	FQuestSceneCutData CutData = UQuestStoryManager::GetQuestSceneCutById(
		SceneListModel->GetCurrentSceneId());

	// Index 번째 이미지를 적용시킨다.
	SceneListView->GetSceneImage()->SetBrushFromTexture(
		CutData.GetSceneImages()[SceneListModel->GetCurrentSceneIndex()]);

	// 모든 처리 이후 값을 1 증가 시킨다.
	SceneListModel->SetCurrentSceneIndex(
		SceneListModel->GetCurrentSceneIndex() + 1);
}
