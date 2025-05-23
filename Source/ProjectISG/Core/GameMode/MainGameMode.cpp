#include "MainGameMode.h"

#include "ProjectISG/Systems/Animation/Manager/LevelSequenceManager.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	ULevelSequenceManager::Initialize();
}
