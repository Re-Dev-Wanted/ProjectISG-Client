#include "UIC_DiaryListUI.h"

#include "JsonObjectConverter.h"
#include "UIM_DiaryListUI.h"
#include "Interfaces/IHttpResponse.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/GameMode/MainGameMode.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_DiaryListUI::InitializeData()
{
	UUIM_DiaryListUI* DiaryListModel = Cast<UUIM_DiaryListUI>(GetModel());

	FGetAllDiariesRequest GetAllDiariesRequest;
	GetAllDiariesRequest.session_id = GetWorld()->GetAuthGameMode<
		AMainGameMode>()->GetSessionId();
	GetAllDiariesRequest.user_id = FSessionUtil::GetCurrentId(GetWorld());

	FApiRequest Request;
	Request.Path = "/log_get_all_diaries";
	FJsonObjectConverter::UStructToJsonObjectString(
		GetAllDiariesRequest, Request.Params);
	// Request.Callback.BindLambda(
	// 	this, [this, DiaryListModel](FHttpRequestPtr Request
	// 								, FHttpResponsePtr Response
	// 								, const bool IsSuccess)
	// 	{
	// 		FApiUtil::DeserializeJsonArray(Response->GetContentAsString()
	// 										, DiaryListModel->DiaryList);
	//
	// 		DiaryListModel->SetCurrentDiaryIndex(
	// 			DiaryListModel->DiaryList.Num());
	// 	});

	FApiUtil::GetMainAPI()->GetApi(this, Request
									, DiaryListModel->GetAllDiariesResponse);
}
