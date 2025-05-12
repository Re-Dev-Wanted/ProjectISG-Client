#include "QuestStoryManager.h"

TMap<FString, FQuestStoryData> UQuestStoryManager::QuestData;

void UQuestStoryManager::Initialize()
{
}

FQuestStoryData& UQuestStoryManager::GetQuestDataById(const FString& QuestId)
{
	return *QuestData.Find(QuestId);
}
