#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/QuestStory/QuestStoryData.h"
#include "UObject/Object.h"
#include "QuestStoryManager.generated.h"

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

private:
	static bool IsInitialized;

	static TMap<FString, FQuestStoryData> QuestData;
	static TMap<FString, TArray<FQuestStoryDialogue>> QuestDialogueData;
	static TMap<FString, FQuestSceneCutData> QuestSceneCutData;

	static void InitializeQuestData();
	static void InitializeQuestDialogue();
	static void InitializeQuestSceneCut();
};
