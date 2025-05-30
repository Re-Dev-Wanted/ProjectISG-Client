#include "UIC_DiaryEditUI.h"

#include "JsonObjectConverter.h"
#include "UIM_DiaryEditUI.h"
#include "UIV_DiaryEditUI.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Contents/Diary/Component/DiaryComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Base/Module/UI_HttpImage.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UUIC_DiaryEditUI::AppearUI()
{
	Super::AppearUI();

	const UUIV_DiaryEditUI* DiaryEditUIView = Cast<UUIV_DiaryEditUI>(GetView());
	DiaryEditUIView->GetSaveButton()->OnClicked.AddDynamic(
		this, &ThisClass::OnClickToSaveDiary);
}

void UUIC_DiaryEditUI::DisappearUI()
{
	Super::DisappearUI();

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->GetDiaryComponent()->NetMulticast_ChangeWriteDiary(true, Player);
}

void UUIC_DiaryEditUI::InitializeDiaryToEdit(
	const FGenerateDiaryResponse& Diary)
{
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(), ATimeManager::StaticClass()));

	const UUIV_DiaryEditUI* DiaryEditUIView = Cast<UUIV_DiaryEditUI>(GetView());

	DiaryEditUIView->GetDiaryDayText()->SetText(
		FText::FromString(FString::FromInt(TimeManager->GetTotalPlayingDay())));
	DiaryEditUIView->GetDiaryDescription()->SetText(
		FText::FromString(Diary.diary));
	DiaryEditUIView->GetGeneratedImage()->SetImagePath(
		Diary.best_screenshot_filename);
}

void UUIC_DiaryEditUI::OnClickToSaveDiary()
{
	const UUIV_DiaryEditUI* DiaryEditUIView = Cast<UUIV_DiaryEditUI>(GetView());
	UUIM_DiaryEditUI* DiaryEditUIModel = Cast<UUIM_DiaryEditUI>(
		GetModel());

	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));

	FSaveDiaryRequest DiaryRequest;

	DiaryRequest.session_id = GetWorld()->GetGameState<AMainGameState>()->
	                                      GetSessionId();
	DiaryRequest.user_id = FSessionUtil::GetCurrentId(GetWorld());
	DiaryRequest.ingame_date = TimeManager->GetDateText();
	DiaryRequest.diary_content = DiaryEditUIView->GetDiaryDescription()->
	                                              GetText().ToString();

	FApiRequest Request;
	Request.Path = TEXT("/diary/save_diary");

	Request.Callback.BindLambda([this](FHttpRequestPtr Req,
	                                   FHttpResponsePtr Res,
	                                   const bool IsSuccess)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			GetView()->GetOwningPlayer());
		PC->GetUIManageComponent()->ResetWidget();
		// TODO: 여기에 일기 저장 이후 추가 로직 필요하면 작성할 것
	});

	FString DataParams;
	FJsonObjectConverter::UStructToJsonObjectString(
		DiaryRequest, DataParams);
	Request.Params = DataParams;

	FApiUtil::GetMainAPI()->PostApi(this, Request,
	                                DiaryEditUIModel->SaveDiaryResponse);
}
