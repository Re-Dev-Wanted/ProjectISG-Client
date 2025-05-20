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
	GETTER(FString, CurrentPlayingSceneId)
	TSet<FString> GetEndQuestIdList() const { return EndQuestIdList; }

	void StartQuest(const FString& NewQuestId);

	void StartScene(const FString& NewSceneId);

	void EndQuest(const bool IsSuccess);

private:
	FString CurrentPlayingQuestId;
	FString CurrentPlayingSceneId;

	// 완료된 퀘스트 아이디 정보들을 저장해준다.
	TSet<FString> EndQuestIdList;

	// Quest Id기반으로 특정 행동에 대한 데이터를 추가한다.
	TMap<FString, TMap<FString, FString>> QuestPlayData;
};
