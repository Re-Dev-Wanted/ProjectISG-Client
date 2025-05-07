#include "UIC_DiaryListUI.h"

#include "JsonObjectConverter.h"
#include "UIM_DiaryListUI.h"
#include "UIV_DiaryListUI.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"
#include "ProjectISG/Core/UI/Base/Module/UI_HttpImage.h"

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
	if (DiaryListModel->GetCurrentDiaryIndex() - 1 < 0)
	{
		return;
	}

	DiaryListModel->SetCurrentDiaryIndex(
		DiaryListModel->GetCurrentDiaryIndex() - 1);

	UpdateDiaryPerPage(DiaryListModel->GetCurrentDiaryIndex());
}

void UUIC_DiaryListUI::MoveToNextPage()
{
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());

	if (DiaryListModel->GetCurrentDiaryIndex() + 1 >= DiaryListModel->DiaryData.
		diaries.Num())
	{
		return;
	}

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
	DiaryListView->GetDiaryImage()->SetImagePath(BestScreenShot);

	// 최소 페이지에 도달한 경우 이전 버튼 미노출 처리
	if (Page == 0)
	{
		DiaryListView->GetPrevButton()->SetRenderOpacity(0);
		DiaryListView->GetPrevButton()->SetIsEnabled(false);
	}
	else
	{
		DiaryListView->GetPrevButton()->SetRenderOpacity(1);
		DiaryListView->GetPrevButton()->SetIsEnabled(true);
	}

	// 최대 페이지에 도달한 경우 다음 페이지 버튼 미노출 처리
	if (Page >= DiaryListModel->DiaryData.diaries.Num() - 1)
	{
		DiaryListView->GetNextButton()->SetRenderOpacity(0);
		DiaryListView->GetNextButton()->SetIsEnabled(false);
	}
	else
	{
		DiaryListView->GetNextButton()->SetRenderOpacity(1);
		DiaryListView->GetNextButton()->SetIsEnabled(true);
	}
}
