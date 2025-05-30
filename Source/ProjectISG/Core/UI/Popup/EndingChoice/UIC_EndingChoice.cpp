// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_EndingChoice.h"

#include "UIM_EndingChoice.h"
#include "UIV_EndingChoice.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/CutSceneList/UIC_SceneListUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

void UUIC_EndingChoice::AppearUI()
{
	Super::AppearUI();

	UUIV_EndingChoice* EndingChoiceView = Cast<UUIV_EndingChoice>(GetView());

	// 버튼 바인드
	EndingChoiceView->GetLeaveThisWorldButton()->OnClicked.AddUniqueDynamic(
		this, &ThisClass::UUIC_EndingChoice::OnLeaveButtonClicked);
	EndingChoiceView->GetRemainThisWorldButton()->OnClicked.AddUniqueDynamic(
		this, &ThisClass::UUIC_EndingChoice::OnRemainButtonClicked);
}

void UUIC_EndingChoice::OnLeaveButtonClicked()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	PC->GetQuestManageComponent()->StartScene(
		EndingChoiceModel->GetLeaveScene());
	UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_SceneListUI]);
	if (SceneListUIController)
	{
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MoveToLobby);
	}
}

void UUIC_EndingChoice::OnRemainButtonClicked()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	PC->GetQuestManageComponent()->StartScene(
		EndingChoiceModel->GetRemainScene());
	UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_SceneListUI]);
	if (SceneListUIController)
	{
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MoveToLobby);
	}
}

void UUIC_EndingChoice::MoveToLobby()
{
	UUIV_EndingChoice* EndingChoiceView = Cast<UUIV_EndingChoice>(GetView());
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());

	UISGGameInstance* ISGGameInstance = EndingChoiceView->GetGameInstance<
		UISGGameInstance>();
	ISGGameInstance->DestroySessionAndMoveLevel(
		EndingChoiceModel->GetLobbyLevel());
}
