#include "MainGameMode.h"

#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"

void AMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GridManagerClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AGridManager* GridManager = GetWorld()->SpawnActor<AGridManager>(GridManagerClass, FTransform::Identity);
		
		if (NewPlayer && NewPlayer->PlayerState)
		{
			if (AMainPlayerState* PlayerState = Cast<AMainPlayerState>(NewPlayer->PlayerState))
			{
				PlayerState->SetGridManager(GridManager);
			}
		}
	}
}
