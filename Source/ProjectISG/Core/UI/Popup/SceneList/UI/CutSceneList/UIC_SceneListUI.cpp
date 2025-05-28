#include "UIC_SceneListUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_SceneListUI.h"
#include "UIV_SceneListUI.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Popup/SceneList/Widget/SceneDialogue/UIC_SceneDialogueWidget.h"
#include "ProjectISG/Core/UI/Popup/SceneList/Widget/SceneDialogue/UIV_SceneDialogueWidget.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_SceneListUI::InitializeController(UBaseUIView* NewView,
                                            UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);
}

void UUIC_SceneListUI::AppearUI()
{
	Super::AppearUI();

	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

	// 현재 씬 아이디를 가져온다.
	SceneListModel->SetCurrentSceneId(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetQuestManageComponent()->GetCurrentPlayingSceneId());

	// 아이디를 가져오면 그 아이디 기반으로 데이터를 가져온다.
	FQuestSceneCutData CutData = UQuestStoryManager::GetQuestSceneCutById(
		SceneListModel->GetCurrentSceneId());

	SceneListModel->SetCurrentSceneIndex(0);
	SceneListModel->SetMaxSceneIndex(CutData.GetSceneCutList().Num());

	MoveToNextScene();
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
		OnEndSceneList();
	}
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

	const UQuestManageComponent* QuestManageComponent = GetView()->
		GetOwningPlayer<
			AMainPlayerController>()->GetQuestManageComponent();

	if (SceneListModel->GetCurrentSceneIndex() >= SceneListModel->
		GetMaxSceneIndex())
	{
		OnEndSceneList();

		return;
	}

	FQuestSceneCutData CutData = UQuestStoryManager::GetQuestSceneCutById(
		SceneListModel->GetCurrentSceneId());

	// Index 번째 이미지를 적용시킨다.
	SceneListView->GetSceneImage()->SetBrushFromTexture(
		CutData.GetSceneCutList()[SceneListModel->GetCurrentSceneIndex()].
		GetSceneImage());

	// 대화하는 상대방의 텍스트를 설정한다.
	const FString OwnerText = CutData.GetSceneCutList()[SceneListModel->
		                          GetCurrentSceneIndex()].
	                          GetDialogueOwner() == TEXT("{Player}")
		                          ? GetView()->GetOwningPlayerState()->
		                                       GetPlayerName()
		                          : CutData.GetSceneCutList()[SceneListModel->
			                          GetCurrentSceneIndex()].
		                          GetDialogueOwner();

	SceneListView->GetSceneDialogue()->GetDialogueOwner()->SetText(
		FText::FromString(OwnerText));

	// 씬 실행
	Cast<UUIC_SceneDialogueWidget>(
			SceneListView->GetSceneDialogue()->GetController())->
		StartSceneDialogue(QuestManageComponent->GetCurrentPlayingSceneId(),
		                   SceneListModel->GetCurrentSceneIndex());

	// 다음 씬 실행 전까지 시간 초 설정
	GetWorld()->GetTimerManager().SetTimer(SceneCutChangeTimerHandle, this,
	                                       &ThisClass::MoveToNextScene,
	                                       CutData.GetSceneCutList()[
		                                       SceneListModel->
		                                       GetCurrentSceneIndex()].
	                                       GetSceneTime(), false);

	// 모든 처리 이후 값을 1 증가 시킨다.
	SceneListModel->SetCurrentSceneIndex(
		SceneListModel->GetCurrentSceneIndex() + 1);
}

void UUIC_SceneListUI::OnEndSceneList()
{
	GetWorld()->GetTimerManager().ClearTimer(SceneCutChangeTimerHandle);
	ResetUIFromPlayerController();

	if (OnSceneListEndNotified.IsBound())
	{
		OnSceneListEndNotified.Execute();
		OnSceneListEndNotified.Unbind();
	}
}
