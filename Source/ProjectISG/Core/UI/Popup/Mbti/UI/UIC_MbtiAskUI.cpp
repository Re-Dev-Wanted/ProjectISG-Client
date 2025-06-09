#include "UIC_MbtiAskUI.h"

#include "JsonObjectConverter.h"
#include "UIM_MbtiAskUI.h"
#include "UIV_MbtiAskUI.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Controller/LobbyPlayerController.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_MbtiAskUI::InitializeController(UBaseUIView* NewView,
                                          UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_MbtiAskUI* MbtiAskUIView = Cast<UUIV_MbtiAskUI>(GetView());

	MbtiAskUIView->GetAnswerTextArea()->OnTextChanged.AddDynamic(
		this, &ThisClass::OnChangeText);
	MbtiAskUIView->GetAnswerTextArea()->OnTextCommitted.AddDynamic(
		this, &ThisClass::OnCommitText);
	MbtiAskUIView->GetSaveButton()->OnClicked.AddDynamic(
		this, &ThisClass::AnswerMbti);
}

void UUIC_MbtiAskUI::OnPushUI()
{
	Super::OnPushUI();

	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());
	MbtiAskUIModel->SetCurrentQuestNum(0);
	MbtiAskUIModel->SetCurrentPercentValue(0);
	AskNewMbti();
}

void UUIC_MbtiAskUI::AskNewMbti()
{
	const UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());
	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(GetView()->GetOwningPlayer());

	Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->
	                                 SetIsReadOnly(true);

	// 이미 종료된 상태에서 재질문을 던지게 되면 UI 종료 후 다음 Flow로 가는 로직이 수행된다.
	if (MbtiAskUIModel->GetCompleted())
	{
		Cast<UUIV_MbtiAskUI>(GetView())->GetAskTextBox()->SetText(
			FText::FromString(MbtiAskUIModel->GetJudged()));
		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->SetText(
			FText::FromString(TEXT("")));
		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->
		                                 SetIsReadOnly(true);
		return;
	}

	LobbyPlayerController->PushUI(EUIName::Modal_TempLoadingUI);

	FApiRequest Request;
	Request.Path = TEXT("/mbti/ask");

	FPostMbtiAskParams Params;
	Params.user_id = FSessionUtil::GetCurrentId(GetWorld());
	Params.session_id = GetWorld()->GetGameInstance<UISGGameInstance>()->
	                                GetSessionId();

	FJsonObjectConverter::UStructToJsonObjectString(Params, Request.Params);

	Request.Callback.BindLambda([this](FHttpRequestPtr Req,
	                                   FHttpResponsePtr Res,
	                                   const bool IsSuccess)
	{
		Cast<ALobbyPlayerController>(GetView()->GetOwningPlayer())->PopUI();

		if (!IsSuccess)
		{
			return;
		}

		FPostMbtiAskResponse MbtiAskResponse;

		FApiUtil::DeserializeJsonObject<FPostMbtiAskResponse>(
			Res->GetContentAsString(), MbtiAskResponse);

		// Model 정보 초기화
		Cast<UUIM_MbtiAskUI>(GetModel())->SetQuestion(MbtiAskResponse.question);
		Cast<UUIM_MbtiAskUI>(GetModel())->SetCompleted(
			MbtiAskResponse.completed);
		Cast<UUIM_MbtiAskUI>(GetModel())->SetMaxQuestionNum(
			MbtiAskResponse.q_num);

		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->SetFocus();
		Cast<UUIM_MbtiAskUI>(GetModel())->
			SetCurrentQuestNum(
				Cast<UUIM_MbtiAskUI>(GetModel())->GetCurrentQuestNum() + 1);

		// UI View Input 초기화
		Cast<UUIV_MbtiAskUI>(GetView())->GetAskTextBox()->SetText(
			FText::FromString(MbtiAskResponse.question));
		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->SetText(
			FText::FromString(TEXT("")));
		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextArea()->
		                                 SetIsReadOnly(false);
		Cast<UUIV_MbtiAskUI>(GetView())->GetCurrentFlowCount()->SetText(
			FText::AsNumber(
				Cast<UUIM_MbtiAskUI>(GetModel())->GetCurrentQuestNum()));
	});

	FApiUtil::GetMainAPI()->PostApi(this, Request, MbtiAskResponse);
}

void UUIC_MbtiAskUI::AnswerMbti()
{
	const UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());

	ALobbyPlayerController* LobbyPlayerController = Cast<
		ALobbyPlayerController>(GetView()->GetOwningPlayer());

	if (MbtiAskUIModel->GetCompleted())
	{
		LobbyPlayerController->PushUI(EUIName::Loading_LoadingUI);
		UGameplayStatics::OpenLevelBySoftObjectPtr(
			GetWorld(), MbtiAskUIModel->GetTrainLevel());
	}

	LobbyPlayerController->PushUI(EUIName::Modal_TempLoadingUI);

	FApiRequest Request;
	Request.Path = TEXT("/mbti/answer");

	FPostMbtiAnswerkParams Params;
	Params.user_id = FSessionUtil::GetCurrentId(GetWorld());
	Params.session_id = GetWorld()->GetGameInstance<UISGGameInstance>()->
	                                GetSessionId();
	Params.response = MbtiAskUIModel->GetAnswer().ToString();

	FJsonObjectConverter::UStructToJsonObjectString(Params, Request.Params);

	Request.Callback.BindLambda([this](FHttpRequestPtr Req,
	                                   FHttpResponsePtr Res,
	                                   const bool IsSuccess)
	{
		Cast<ALobbyPlayerController>(GetView()->GetOwningPlayer())->PopUI();

		FPostMbtiAnswerResponse MbtiAnswerResponse;

		FApiUtil::DeserializeJsonObject<FPostMbtiAnswerResponse>(
			Res->GetContentAsString(), MbtiAnswerResponse);

		Cast<UUIM_MbtiAskUI>(GetModel())->SetCompleted(
			MbtiAnswerResponse.completed);
		Cast<UUIM_MbtiAskUI>(GetModel())->SetJudged(MbtiAnswerResponse.judged);

		AskNewMbti();
	});

	FApiUtil::GetMainAPI()->PostApi(this, Request, MbtiAskResponse);
}

void UUIC_MbtiAskUI::OnChangeText(const FText& Text)
{
	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());
	MbtiAskUIModel->SetAnswer(Text);
}

void UUIC_MbtiAskUI::OnCommitText(const FText& Text,
                                  ETextCommit::Type CommitMethod)
{
	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());
	MbtiAskUIModel->SetAnswer(Text);

	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		AnswerMbti();
	}
}
