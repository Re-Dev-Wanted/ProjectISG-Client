#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "QuestManageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UQuestManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestManageComponent();

	GETTER(FString, CurrentPlayingQuestId)

	void StartQuest(const FString& NewQuestId);

	void EndQuest(const bool IsSuccess);

private:
	FString CurrentPlayingQuestId;
};
