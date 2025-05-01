#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class PROJECTISG_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class AGridManager> GridManagerClass;

protected:
	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
