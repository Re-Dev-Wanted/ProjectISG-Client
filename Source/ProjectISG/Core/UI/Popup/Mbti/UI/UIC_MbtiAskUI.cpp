#include "UIC_MbtiAskUI.h"

#include "JsonObjectConverter.h"
#include "UIM_MbtiAskUI.h"
#include "UIV_MbtiAskUI.h"
#include "Components/EditableText.h"
#include "Components/MultiLineEditableTextBox.h"
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

	MbtiAskUIView->GetAnswerTextBox()->OnTextChanged.AddDynamic(
		this, &ThisClass::OnChangeText);
	MbtiAskUIView->GetAnswerTextBox()->OnTextCommitted.AddDynamic(
		this, &ThisClass::OnCommitText);
}

void UUIC_MbtiAskUI::AppearUI()
{
	Super::AppearUI();

	AskNewMbti();
}

void UUIC_MbtiAskUI::AskNewMbti()
{
	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());
	// 이미 종료된 상태에서 재질문을 던지게 되면 UI 종료 후 다음 Flow로 가는 로직이 수행된다.
	if (MbtiAskUIModel->GetCompleted())
	{
		//GetView()->GetOwningPlayer<AMainPlayerController>()->PopUI();
		// TODO: Delegate로 이후 동작을 받아오기
		ALobbyPlayerController* LobbyPlayerController = Cast<
			ALobbyPlayerController>(GetView()->GetOwningPlayer());
		LobbyPlayerController->PushUI(EUIName::Loading_LoadingUI);
		UGameplayStatics::OpenLevelBySoftObjectPtr(
			GetWorld(), MbtiAskUIModel->GetTrainLevel());

		return;
	}

	FApiRequest Request;
	Request.Path = "/mbti/ask";

	FPostMbtiAskParams Params;
	Params.user_id = FSessionUtil::GetCurrentId(GetWorld());
	Params.session_id = GetWorld()->GetGameInstance<UISGGameInstance>()->
	                                GetSessionId();

	FJsonObjectConverter::UStructToJsonObjectString(Params, Request.Params);

	Request.Callback.BindLambda([this](FHttpRequestPtr Req,
	                                   FHttpResponsePtr Res,
	                                   const bool IsSuccess)
	{
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

		// UI View Input 초기화
		Cast<UUIV_MbtiAskUI>(GetView())->GetAskTextBox()->SetText(
			FText::FromString(MbtiAskResponse.question));
		Cast<UUIV_MbtiAskUI>(GetView())->GetAnswerTextBox()->SetText(
			FText::FromString(TEXT("")));
	});

	FApiUtil::GetMainAPI()->PostApi(this, Request, MbtiAskResponse);
}

void UUIC_MbtiAskUI::AnswerMbti()
{
	UUIV_MbtiAskUI* MbtiAskUIView = Cast<UUIV_MbtiAskUI>(GetView());
	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());

	FApiRequest Request;
	Request.Path = "/mbti/answer";

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
		FPostMbtiAnswerResponse MbtiAnswerResponse;

		FApiUtil::DeserializeJsonObject<FPostMbtiAnswerResponse>(
			Res->GetContentAsString(), MbtiAnswerResponse);

		Cast<UUIM_MbtiAskUI>(GetModel())->SetCompleted(
			MbtiAnswerResponse.completed);

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
