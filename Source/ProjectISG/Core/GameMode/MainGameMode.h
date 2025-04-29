#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainGameMode.generated.h"

UCLASS()
class PROJECTISG_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class AGridManager> GridManagerClass;

	GETTER(FString, SessionId)

protected:
	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	// TODO: 임시용 로컬 데이터로 추후 값을 API or 저장된 데이터로 받아야한다.
	FString SessionId = TEXT("e1827901-2536-4fb9-b76a-ca8e149015cb");
};
