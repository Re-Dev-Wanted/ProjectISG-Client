// Fill out your copyright notice in the Description page of Project Settings.


#include "UIC_EndingChoice.h"

#include "UIM_EndingChoice.h"
#include "UIV_EndingChoice.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/CutSceneList/UIC_SceneListUI.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/MediaSceneList/UIC_MediaSceneListUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

void UUIC_EndingChoice::AppearUI()
{
	Super::AppearUI();

	UUIV_EndingChoice* EndingChoiceView = Cast<UUIV_EndingChoice>(GetView());

	// 버튼 바인드
	EndingChoiceView->GetLeaveThisWorldButton()->OnClicked.AddDynamic(
		this, &ThisClass::UUIC_EndingChoice::OnLeaveButtonClicked);
	EndingChoiceView->GetRemainThisWorldButton()->OnClicked.AddDynamic(
		this, &ThisClass::UUIC_EndingChoice::OnRemainButtonClicked);
}

void UUIC_EndingChoice::OnLeaveButtonClicked()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	PC->PopUI();
	
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	
	// PC->GetQuestManageComponent()->StartScene(
	// 	EndingChoiceModel->GetEndingVideoScene());
	// UUIC_MediaSceneListUI* MediaSceneListUIController = Cast<UUIC_MediaSceneListUI>(
	// 	PC->GetUIManageComponent()->ControllerInstances[
	// 		EUIName::Popup_MediaSceneListUI]);
	// if (MediaSceneListUIController)
	// {
	// 	MediaSceneListUIController->OnEndMediaSceneNotified.BindDynamic(this, &ThisClass::EndingLeaveSceneStart);
	// }

	PC->GetQuestManageComponent()->StartScene(EndingChoiceModel->GetLeaveScene());
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
	PC->PopUI();
	
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	
	// PC->GetQuestManageComponent()->StartScene(
	// 	EndingChoiceModel->GetEndingVideoScene());
	// UUIC_MediaSceneListUI* MediaSceneListUIController = Cast<UUIC_MediaSceneListUI>(
	// 	PC->GetUIManageComponent()->ControllerInstances[
	// 		EUIName::Popup_MediaSceneListUI]);
	// if (MediaSceneListUIController)
	// {
	// 	MediaSceneListUIController->OnEndMediaSceneNotified.BindDynamic(this, &ThisClass::EndingRemainSceneStart);
	// }

	PC->GetQuestManageComponent()->StartScene(EndingChoiceModel->GetRemainScene());
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
	UE_LOG(LogTemp, Warning, TEXT("test"));
	UUIV_EndingChoice* EndingChoiceView = Cast<UUIV_EndingChoice>(GetView());
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());

	UISGGameInstance* ISGGameInstance = EndingChoiceView->GetGameInstance<
		UISGGameInstance>();
	ISGGameInstance->DestroySessionAndMoveLevel(
		EndingChoiceModel->GetLobbyLevel());
}

void UUIC_EndingChoice::EndingLeaveSceneStart()
{
	UE_LOG(LogTemp, Warning, TEXT("EndingLeaveSceneStart()"));

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	PC->GetQuestManageComponent()->StartScene(EndingChoiceModel->GetLeaveScene());
	UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_SceneListUI]);
	if (SceneListUIController)
	{
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MoveToLobby);
	}
}

void UUIC_EndingChoice::EndingRemainSceneStart()
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetPlayerController());
	
	UUIM_EndingChoice* EndingChoiceModel = Cast<UUIM_EndingChoice>(GetModel());
	PC->GetQuestManageComponent()->StartScene(EndingChoiceModel->GetRemainScene());
	UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_SceneListUI]);
	if (SceneListUIController)
	{
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MoveToLobby);
	}
}
