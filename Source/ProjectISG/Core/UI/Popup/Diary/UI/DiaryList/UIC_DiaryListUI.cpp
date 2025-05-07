#include "UIC_DiaryListUI.h"

#include "JsonObjectConverter.h"
#include "UIM_DiaryListUI.h"
#include "UIV_DiaryListUI.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/GameMode/MainGameMode.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_DiaryListUI::InitializeController(UBaseUIView* NewView
                                            , UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	const UUIV_DiaryListUI* DiaryListView = Cast<UUIV_DiaryListUI>(GetView());

	DiaryListView->GetPrevButton()->OnClicked.AddDynamic(
		this, &ThisClass::MoveToPrevPage);

	DiaryListView->GetNextButton()->OnClicked.AddDynamic(
		this, &ThisClass::MoveToNextPage);

	InitializeData();
}

void UUIC_DiaryListUI::InitializeData()
{
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());

	FGetAllDiariesRequest GetAllDiariesRequest;
	GetAllDiariesRequest.session_id = GetWorld()->GetGameState<AMainGameState>()
	                                            ->GetSessionId();
	GetAllDiariesRequest.user_id = FSessionUtil::GetCurrentId(GetWorld());

	FApiRequest Request;
	Request.Path = "/diary/get_all_diaries";
	FJsonObjectConverter::UStructToJsonObjectString(
		GetAllDiariesRequest, Request.Params);

	Request.Callback.BindLambda(
		[this](FHttpRequestPtr Request, FHttpResponsePtr Response
		       , const bool IsSuccess)
		{
			UUIM_DiaryListUI* ResponseDiaryListModel = Cast<UUIM_DiaryListUI>(
				GetModel());

			FApiUtil::DeserializeJsonObject<FGetAllDiariesResponse>(
				Response->GetContentAsString()
				, ResponseDiaryListModel->DiaryData);

			ResponseDiaryListModel->SetCurrentDiaryIndex(
				ResponseDiaryListModel->DiaryData.diaries.Num() - 1);

			UpdateDiaryPerPage(ResponseDiaryListModel->GetCurrentDiaryIndex());
		});

	FApiUtil::GetMainAPI()->PostApi(this, Request
	                                , DiaryListModel->GetAllDiariesResponse);
}

void UUIC_DiaryListUI::MoveToPrevPage()
{
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());
	DiaryListModel->SetCurrentDiaryIndex(
		DiaryListModel->GetCurrentDiaryIndex() - 1);

	UpdateDiaryPerPage(DiaryListModel->GetCurrentDiaryIndex());
}

void UUIC_DiaryListUI::MoveToNextPage()
{
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());
	DiaryListModel->SetCurrentDiaryIndex(
		DiaryListModel->GetCurrentDiaryIndex() + 1);

	UpdateDiaryPerPage(DiaryListModel->GetCurrentDiaryIndex());
}

void UUIC_DiaryListUI::UpdateDiaryPerPage(const int Page)
{
	const UUIV_DiaryListUI* DiaryListView = Cast<UUIV_DiaryListUI>(GetView());
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());

	const auto [DiaryId, InGameDate, Content, BestScreenShot] = DiaryListModel->
		DiaryData.diaries[Page];

	DiaryListView->GetDiaryDayText()->SetText(FText::FromString(InGameDate));
	DiaryListView->GetDiaryDescription()->SetText(FText::FromString(Content));

	DiaryListView->GetPrevButton()->SetVisibility(
		DiaryListModel->DiaryData.diaries.Num() - 1 <= Page
			? ESlateVisibility::Hidden
			: ESlateVisibility::Visible);

	DiaryListView->GetNextButton()->SetVisibility(
		DiaryListModel->DiaryData.diaries.Num() - 1 >= Page
			? ESlateVisibility::Hidden
			: ESlateVisibility::Visible);
}
