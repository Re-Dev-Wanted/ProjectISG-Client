#include "QuestStoryManager.h"

TMap<FString, FQuestStoryData> UQuestStoryManager::QuestData;
TMap<FString, TArray<FQuestStoryDialogue>>
UQuestStoryManager::QuestDialogueData;
TMap<FString, FQuestSceneCutData> UQuestStoryManager::QuestSceneCutData;

bool UQuestStoryManager::IsInitialized;

void UQuestStoryManager::Initialize()
{
	if (!IsInitialized)
	{
		IsInitialized = true;

		InitializeQuestData();
		InitializeQuestDialogue();
		InitializeQuestSceneCut();
	}
}

FQuestStoryData& UQuestStoryManager::GetQuestDataById(const FString& QuestId)
{
	return *QuestData.Find(QuestId);
}

TArray<FQuestStoryDialogue>& UQuestStoryManager::GetQuestDialogueById(
	const FString& QuestId)
{
	return *QuestDialogueData.Find(QuestId);
}

FQuestSceneCutData& UQuestStoryManager::GetQuestSceneCutById(
	const FString& SceneId)
{
	return *QuestSceneCutData.Find(SceneId);
}

void UQuestStoryManager::InitializeQuestData()
{
	const UDataTable* QuestStoryDataTable = LoadObject<UDataTable>(
		nullptr,
		TEXT(
			"/Script/Engine.DataTable'/Game/Systems/QuestStory/Data/DT_QuestStoryData.DT_QuestStoryData'"));

	TArray<FQuestStoryData*> TempQuestStoryData;
	QuestStoryDataTable->GetAllRows<FQuestStoryData>(
		TEXT(""), TempQuestStoryData);

	for (const FQuestStoryData* QuestStoryData : TempQuestStoryData)
	{
		QuestData.Add(QuestStoryData->GetQuestId(), *QuestStoryData);
	}
}

void UQuestStoryManager::InitializeQuestDialogue()
{
	const UDataTable* QuestStoryDialogueTable = LoadObject<UDataTable>(
		nullptr,
		TEXT(
			"/Script/Engine.DataTable'/Game/Systems/QuestStory/Data/DT_QuestStoryDialogue.DT_QuestStoryDialogue'"));

	TArray<FQuestStoryDialogue*> TempQuestStoryDialogueTable;
	QuestStoryDialogueTable->GetAllRows<FQuestStoryDialogue>(
		TEXT(""), TempQuestStoryDialogueTable);

	// 초기 데이터 삽입
	for (const FQuestStoryDialogue* StoryDialogueData :
	     TempQuestStoryDialogueTable)
	{
		if (QuestDialogueData.Contains(StoryDialogueData->GetQuestId()))
		{
			QuestDialogueData[StoryDialogueData->GetQuestId()].Add(
				*StoryDialogueData);
		}
		else
		{
			TArray<FQuestStoryDialogue> NewTempData;
			NewTempData.Add(*StoryDialogueData);

			QuestDialogueData.Add(StoryDialogueData->GetQuestId(), NewTempData);
		}
	}

	// 초기 데이터 정렬
	for (const auto DialogueData : QuestDialogueData)
	{
		QuestDialogueData[DialogueData.Key].HeapSort(
			[](const FQuestStoryDialogue& A, const FQuestStoryDialogue& B)
			{
				return A.GetDialogueIndex() < B.GetDialogueIndex();
			});
	}
}

void UQuestStoryManager::InitializeQuestSceneCut()
{
	const UDataTable* QuestSceneCutDataTable = LoadObject<UDataTable>(
		nullptr,
		TEXT(
			"/Script/Engine.DataTable'/Game/Systems/QuestStory/Data/DT_QuestSceneCutData.DT_QuestSceneCutData'"));

	TArray<FQuestSceneCutData*> TempQuestSceneCutList;
	QuestSceneCutDataTable->GetAllRows<FQuestSceneCutData>(
		TEXT(""), TempQuestSceneCutList);

	for (const FQuestSceneCutData* TempQuestSceneCutData :
	     TempQuestSceneCutList)
	{
		QuestSceneCutData.Add(TempQuestSceneCutData->GetQuestSceneId(),
		                      *TempQuestSceneCutData);
	}
}
