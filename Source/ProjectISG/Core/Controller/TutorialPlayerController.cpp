#include "TutorialPlayerController.h"

#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/UIC_SceneListUI.h"


void ATutorialPlayerController::BeginPlay()
{
	GetUIManageComponent()->Initialize();
	GetUIManageComponent()->PushWidget(EUIName::Gameplay_MainHUD);
	if (StartSceneName.IsEmpty() == false)
	{
		StartScene(StartSceneName);
		UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(GetUIManageComponent()->ControllerInstances[EUIName::Popup_SceneListUI]);
		SceneListUIController->OnSceneListEndNotified.BindUObject(this, &ThisClass::StartSceneEnd);
	}
}

void ATutorialPlayerController::StartSceneEnd()
{
	if (!StartQuestId.IsEmpty())
	{
		StartQuest(StartQuestId);
	}
}

void ATutorialPlayerController::StartScene6(bool IsServerTravel)
{
	bIsServerTravel = IsServerTravel;
	StartScene(FString::Printf(TEXT("Scene_6")));
	UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(GetUIManageComponent()->ControllerInstances[EUIName::Popup_SceneListUI]);
	SceneListUIController->OnSceneListEndNotified.BindUObject(this, &ThisClass::OpenMainLevel);
}

void ATutorialPlayerController::OpenMainLevel()
{
	ShowLoadingUIAndCreateSession(bIsServerTravel);
}
