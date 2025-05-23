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

	static TArray<FQuestRequireData> GetRequireQuestDataById(
		const FString& QuestId);

	static uint32 GetRequireQuestDateToAbleFinish(
		const AMainPlayerController* PC, const FString& QuestId);

	static bool CheckAndCompleteQuest(AMainPlayerController* PC,
	                                  const FString& QuestId);

	static FORCEINLINE TArray<FQuestStoryData>& GetAllQuestData()
	{
		return QuestArrayList;
	}

private:
	static bool IsInitialized;

	static TMap<FString, FQuestStoryData> QuestData;
	static TArray<FQuestStoryData> QuestArrayList;
	static TMap<FString, TArray<FQuestStoryDialogue>> QuestDialogueData;
	static TMap<FString, FQuestSceneCutData> QuestSceneCutData;
	static TMap<FString, FQuestItemReward> QuestRewardItemData;
	static TMap<FString, TArray<FQuestRequireData>> QuestRequireData;

	static void InitializeQuestData();
	static void InitializeQuestRequireData();
	static void InitializeQuestDialogue();
	static void InitializeQuestSceneCut();

	static TArray<FString>
	GetQuestRequiredItemTableById(const FString& QuestId);

	static void CompleteQuest_Internal(AMainPlayerController* PC,
	                                   const FString& QuestId);

	static void GiveRewardQuest_Internal(AMainPlayerController* PC,
	                                     const FString& QuestId);
};
