#include "TutorialPlayerController.h"


void ATutorialPlayerController::BeginPlay()
{
	Super::BeginPlay();

	StartScene(FString::Printf(TEXT("Scene_1")));
}

void ATutorialPlayerController::StartGranmaLevel()
{
	StartScene(FString::Printf(TEXT("Scene_3")));
}
