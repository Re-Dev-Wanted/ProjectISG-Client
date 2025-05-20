#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/QuestStory/QuestStoryData.h"
#include "UObject/Object.h"
#include "QuestStoryManager.generated.h"

class AMainPlayerController;

UCLASS()
class PROJECTISG_API UQuestStoryManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FQuestStoryData& GetQuestDataById(const FString& QuestId);
	static TArray<FQuestStoryDialogue>& GetQuestDialogueById(
		const FString& QuestId);
	static FQuestSceneCutData& GetQuestSceneCutById(const FString& SceneId);

	static bool CheckAndCompleteQuest(AMainPlayerController* PC,
	                                  const FString& QuestId);

	static FORCEINLINE TArray<FQuestStoryData>& GetAllQuestData()
	{
		return QuestArrayList;
	}

	// 퀘스트에서 수행해야 할 모든 행동 퀘스트의 갯수를 가져온다.
	static uint32 GetQuestAllBehaviorCount(const FString& QuestId);

private:
	static bool IsInitialized;

	static TMap<FString, FQuestStoryData> QuestData;
	static TArray<FQuestStoryData> QuestArrayList;
	static TMap<FString, TArray<FQuestStoryDialogue>> QuestDialogueData;
	static TMap<FString, FQuestSceneCutData> QuestSceneCutData;

	static void InitializeQuestData();
	static void InitializeQuestDialogue();
	static void InitializeQuestSceneCut();

	static TArray<FString>
	GetQuestRequiredItemTableById(const FString& QuestId);

	static void CompleteQuest_Internal(AMainPlayerController* PC,
	                                   const FString& QuestId);
};
