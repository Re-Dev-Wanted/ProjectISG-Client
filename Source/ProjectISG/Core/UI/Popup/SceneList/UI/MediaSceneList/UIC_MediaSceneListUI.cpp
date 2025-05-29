#include "UIC_MediaSceneListUI.h"
#include "UIV_MediaSceneListUI.h"

#include "EnhancedInputComponent.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "Components/SynthComponent.h"
#include "UIM_MediaSceneListUI.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_MediaSceneListUI::OnPushUI()
{
	Super::OnPushUI();

	// 해당 타입이 무조건 Media 타입이라고 가정하고 한다.
	// 잘못된 타입일 시 그냥 터트리는게 맞음.
	const UUIV_MediaSceneListUI* SceneListView = Cast<UUIV_MediaSceneListUI>(
		GetView());
	UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());

	// 미디어 관련 정보 가져오기
	SceneListModel->SetCurrentSceneId(
		GetView()->GetOwningPlayer<AMainPlayerController>()->
		           GetQuestManageComponent()->GetCurrentPlayingSceneId());

	FQuestSceneCutData MediaSceneData =
		UQuestStoryManager::GetQuestSceneCutById(
			SceneListModel->GetCurrentSceneId());

	const TObjectPtr<UMediaPlayer> MediaPlayer = MediaSceneData.GetSceneMedia().
		GetSceneMediaPlayer();

	// 정보를 기반으로 Material 세팅
	SceneListView->GetSceneImage()->SetBrushFromMaterial(
		MediaSceneData.GetSceneMedia().GetSceneMediaTexture());

	// 혹시 이미 재생중이면 일시정지 후 종료로 초기화한다.
	if (MediaPlayer->IsPlaying())
	{
		// 정상적인 종료가 아니기에 Delegate를 날려준다.
		MediaPlayer->OnEndReached.Clear();

		MediaPlayer->Pause();
		MediaPlayer->Close();
	}

	// Delegate 에 대한 설정이 없다면 여기서 처리해준다.
	if (!MediaPlayer->OnMediaOpened.IsBound())
	{
		MediaPlayer->OnMediaOpened.AddDynamic(
			this, &ThisClass::OnOpenedMediaScene);
	}

	if (!MediaPlayer->OnEndReached.IsBound())
	{
		MediaPlayer->OnEndReached.AddDynamic(this, &ThisClass::OnEndMediaScene);
	}

	// 미디어 재생 준비를 시전한다.
	MediaPlayer->OpenSource(
		MediaSceneData.GetSceneMedia().GetSceneMediaSource());
}

void UUIC_MediaSceneListUI::OnOpenedMediaScene(FString OpenUrl)
{
	const UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetView()->GetOwningPlayerPawn());

	FQuestSceneCutData MediaSceneData =
		UQuestStoryManager::GetQuestSceneCutById(
			SceneListModel->GetCurrentSceneId());

	// 미디어를 재생한다.
	MediaSceneData.GetSceneMedia().GetSceneMediaPlayer()->Play();

	// 미디어에 맞는 소리 또한 세팅해주고 재생해준다.
	Player->GetMediaSoundComponent()->SetMediaPlayer(
		MediaSceneData.GetSceneMedia().GetSceneMediaPlayer());
	Player->GetMediaSoundComponent()->Start();
}

void UUIC_MediaSceneListUI::OnEndMediaScene()
{
	ResetUIFromPlayerController();

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetView()->GetOwningPlayerPawn());
	Player->GetMediaSoundComponent()->Stop();

	if (OnEndMediaSceneNotified.IsBound())
	{
		OnEndMediaSceneNotified.Execute();
		OnEndMediaSceneNotified.Clear();
	}
}

void UUIC_MediaSceneListUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Triggered, this
	                           , &ThisClass::OnTriggerSkipSceneAction);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Completed, this
	                           , &ThisClass::OnEndSkipSceneAction);
}

void UUIC_MediaSceneListUI::OnTriggerSkipSceneAction()
{
	const UUIV_MediaSceneListUI* SceneListView = Cast<UUIV_MediaSceneListUI>(
		GetView());
	UUIM_MediaSceneListUI* SceneListModel = Cast<UUIM_MediaSceneListUI>(
		GetModel());
	FQuestSceneCutData MediaSceneData =
		UQuestStoryManager::GetQuestSceneCutById(
			SceneListModel->GetCurrentSceneId());

	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Visible);
	SceneListModel->SetCurrentLoadingPercent(
		SceneListModel->GetCurrentLoadingPercent() + 1);

	const float CurrentPercent = SceneListModel->GetCurrentLoadingPercent();

	SetSkipCircularPercent(
		CurrentPercent / SceneListModel->GetMaxLoadingPercent());

	if (SceneListModel->GetCurrentLoadingPercent() >= SceneListModel->
		GetMaxLoadingPercent())
	{
		MediaSceneData.GetSceneMedia().GetSceneMediaPlayer()->Close();
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
