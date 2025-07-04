#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "QuestManageComponent.generated.h"

enum class EQuestStatus : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UQuestManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestManageComponent();

	GETTER(FString, CurrentPlayingQuestId)
	GETTER(FString, CurrentPlayingSceneId)

	TSet<FString> GetCompletedQuestIdList() const
	{
		return CompletedQuestIdList;
	}

	void StartQuest(const FString& NewQuestId);

	UFUNCTION(BlueprintCallable)
	void StartScene(const FString& NewSceneId);

	UFUNCTION(BlueprintCallable)
	bool EndQuest(const bool IsCheckedQuestEnd = false);

	EQuestStatus GetQuestStatusById(const FString& QuestId) const;

private:
	FString CurrentPlayingQuestId;
	FString CurrentPlayingSceneId;

	// 완료된 퀘스트 아이디 정보들을 저장해준다.
	TSet<FString> CompletedQuestIdList;

	// Quest Id기반으로 특정 행동에 대한 데이터를 추가한다.
	TMap<FString, TMap<FString, FString>> QuestPlayData;
};
