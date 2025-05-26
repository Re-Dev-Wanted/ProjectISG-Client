#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainGameState.generated.h"

class UQuestManageComponent;

UCLASS()
class PROJECTISG_API AMainGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMainGameState();

	GETTER_SETTER(FString, SessionId);

	GETTER_SETTER(FString, CurrentWorldQuestId)

	void StartWorldQuest(const FString& QuestId);

	void EndWorldQuest();

protected:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, VisibleAnywhere)
	FString SessionId = TEXT("e1827901-2536-4fb9-b76a-ca8e149015cb");

	UPROPERTY(Replicated)
	FString CurrentWorldQuestId;
};
