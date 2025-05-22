#include "TutorialPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/UIC_SceneListUI.h"


void ATutorialPlayerController::BeginPlay()
{
	GetUIManageComponent()->Initialize();
	GetUIManageComponent()->PushWidget(EUIName::Gameplay_MainHUD);
	StartScene(StartSceneName);
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
