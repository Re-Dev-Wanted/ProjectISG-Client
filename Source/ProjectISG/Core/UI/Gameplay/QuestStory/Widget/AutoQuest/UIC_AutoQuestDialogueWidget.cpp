#include "UIC_AutoQuestDialogueWidget.h"

#include "UIM_AutoQuestDialogueWidget.h"
#include "UIV_AutoQuestDialogueWidget.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

void UUIC_AutoQuestDialogueWidget::InitializeController(
	UBaseUIView* NewView, UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	UUIV_AutoQuestDialogueWidget* AutoQuestDialogueWidgetView = Cast<
		UUIV_AutoQuestDialogueWidget>(NewView);

	// 실질적으로 얘가 재귀를 시켜
	AutoQuestDialogueWidgetView->OnDialogueEndNotified.BindDynamic(
		this, &ThisClass::OnFinishDialogue);

	AutoQuestDialogueWidgetView->BindToAnimationFinished(
		AutoQuestDialogueWidgetView->GetDialogueAnimation()
		, AutoQuestDialogueWidgetView->OnDialogueEndNotified);
}

void UUIC_AutoQuestDialogueWidget::InitializeDialogue()
{
	UUIV_AutoQuestDialogueWidget* AutoQuestDialogueWidgetView = Cast<
		UUIV_AutoQuestDialogueWidget>(GetView());
	UUIM_AutoQuestDialogueWidget* AutoQuestDialogueWidgetModel = Cast<
		UUIM_AutoQuestDialogueWidget>(GetModel());

	const uint8 CurrentQuestDialogueIndex = AutoQuestDialogueWidgetModel->
		GetCurrentQuestDialogueIndex();

	const UQuestManageComponent* PlayerQuestManager = Cast<
			AMainPlayerController>(
			AutoQuestDialogueWidgetView->GetOwningPlayer())->
		GetQuestManageComponent();

	TArray<FQuestStoryDialogue> Dialogues =
		UQuestStoryManager::GetQuestDialogueById(
			PlayerQuestManager->GetCurrentPlayingQuestId());


	// 대사가 없는 Dialogue 예외처리
	if (Dialogues.Num() == 0)
	{
		OnFinishDialogue();
		return;
	}
	
	// 대사가 없으면 대사를 종료시킨다.
	if (AutoQuestDialogueWidgetModel->GetCurrentQuestDialogueIndex() >= Dialogues.Num())
	{
		OnFinishDialogue();
		return;
	}

	// 대사를 말하는 주체에 대한 이름 설정
	const FString OwnerText = Dialogues[CurrentQuestDialogueIndex].
	                          GetDialogueOwner() == TEXT("{Player}")
		                          ? GetView()->GetOwningPlayerState()->
		                                       GetPlayerName()
		                          : Dialogues[CurrentQuestDialogueIndex].
		                          GetDialogueOwner();

	AutoQuestDialogueWidgetView->GetDialogueOwner()->SetText(
		FText::FromString(OwnerText));
	AutoQuestDialogueWidgetView->GetDialogueText()->SetText(
		Dialogues[CurrentQuestDialogueIndex].GetDialogueText());
	AutoQuestDialogueWidgetView->PlaySound(
		Dialogues[CurrentQuestDialogueIndex].GetDialogueTTS());
	AutoQuestDialogueWidgetView->PlayAnimation(AutoQuestDialogueWidgetView->GetDialogueAnimation());

	TWeakObjectPtr<ThisClass> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(QuestDialogueChangeTimerHandle, 
										   [WeakThis]()
										   {
												   if (WeakThis.IsValid())
												   {
													   WeakThis.Get()->InitializeDialogue();
												   }
										   },
										   Dialogues[CurrentQuestDialogueIndex].
										   GetDialogueTime(), false);

	// 모든 일련의 과정이 완료되면 1을 더해 다음 대사를 준비한다.
	AutoQuestDialogueWidgetModel->SetCurrentQuestDialogueIndex(
		AutoQuestDialogueWidgetModel->GetCurrentQuestDialogueIndex() + 1);
}

void UUIC_AutoQuestDialogueWidget::StartQuestDialogue()
{
	UUIM_AutoQuestDialogueWidget* AutoQuestDialogueWidgetModel = Cast<
		UUIM_AutoQuestDialogueWidget>(GetModel());

	AutoQuestDialogueWidgetModel->SetCurrentQuestDialogueIndex(0);
	InitializeDialogue();
}

void UUIC_AutoQuestDialogueWidget::SkipQuestDialogue()
{
	UUIV_AutoQuestDialogueWidget* AutoQuestDialogueWidgetView = Cast<
		UUIV_AutoQuestDialogueWidget>(GetView());

	AutoQuestDialogueWidgetView->StopAnimation(
		AutoQuestDialogueWidgetView->GetDialogueAnimation());

	AutoQuestDialogueWidgetView->SetVisibility(ESlateVisibility::Hidden);
}

void UUIC_AutoQuestDialogueWidget::OnFinishDialogue()
{
	UUIM_AutoQuestDialogueWidget* AutoQuestDialogueWidgetModel = Cast<
		UUIM_AutoQuestDialogueWidget>(GetModel());

	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());
	UQuestManageComponent* PlayerQuestManager = PC->
		GetQuestManageComponent();

	// 현재 총 퀘스트의 대화 배열 수
	const uint8 DialogueCount = UQuestStoryManager::GetQuestDialogueById(
		PlayerQuestManager->GetCurrentPlayingQuestId()).Num();

	if (PlayerQuestManager->GetCurrentPlayingQuestId() == TEXT(""))
	{
		AutoQuestDialogueWidgetModel->SetCurrentQuestDialogueIndex(0);
		return;
	}

	// 대화 배열 수 이상까지 가면 더 이상 출력할 대사 없음
	// 또한 퀘스트 북을 통하지 않는 퀘스트의 경우 (ex. 튜토리얼 케이스)
	// 에 대사 완료 이후 완료 여부를 검증해 퀘스트를 종료한다.
	if (AutoQuestDialogueWidgetModel->GetCurrentQuestDialogueIndex() >=
		DialogueCount)
	{
		PC->GetMainHUD()->ToggleAutoQuestUI(false);
		return;
	}
}
