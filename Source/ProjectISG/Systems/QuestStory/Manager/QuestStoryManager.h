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

private:
	static bool IsInitialized;

	static TMap<FString, FQuestStoryData> QuestData;
	static TArray<FQuestStoryData> QuestArrayList;
	static TMap<FString, TArray<FQuestStoryDialogue>> QuestDialogueData;
	static TMap<FString, FQuestSceneCutData> QuestSceneCutData;

	static void InitializeQuestData();
	static void InitializeQuestDialogue();
	static void InitializeQuestSceneCut();

	static void CompleteQuest_Internal(AMainPlayerController* PC,
	                                   const FString& QuestId);
};
