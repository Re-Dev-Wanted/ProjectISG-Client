#include "QuestStoryManager.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

TArray<FQuestStoryData> UQuestStoryManager::QuestArrayList;
TMap<FString, FQuestStoryData> UQuestStoryManager::QuestData;
TMap<FString, TArray<FQuestRequireData>> UQuestStoryManager::QuestRequireData;
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
		InitializeQuestRequireData();
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
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestStoryDataTable(
			TEXT(
				"'/Game/Systems/QuestStory/Data/DT_QuestStoryData.DT_QuestStoryData'"));

	if (!QuestStoryDataTable.Succeeded())
	{
		return;
	}

	TArray<FQuestStoryData*> TempQuestStoryData;
	QuestStoryDataTable.Object->GetAllRows<FQuestStoryData>(
		TEXT(""), TempQuestStoryData);

	TempQuestStoryData.HeapSort(
		[](const FQuestStoryData& A, const FQuestStoryData& B)
		{
			return A.GetQuestId() < B.GetQuestId();
		});

	for (const FQuestStoryData* QuestStoryData : TempQuestStoryData)
	{
		QuestArrayList.Add(*QuestStoryData);
		QuestData.Add(QuestStoryData->GetQuestId(), *QuestStoryData);
	}
}

void UQuestStoryManager::InitializeQuestRequireData()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestRequireDataTable(
			TEXT(
				"'/Game/Systems/QuestStory/Data/DT_QuestRequireData.DT_QuestRequireData'"));

	if (!QuestRequireDataTable.Succeeded())
	{
		return;
	}

	TArray<FQuestRequireData*> TempQuestRequireData;
	QuestRequireDataTable.Object->GetAllRows<FQuestRequireData>(
		TEXT(""), TempQuestRequireData);

	for (const FQuestRequireData* QuestRequire : TempQuestRequireData)
	{
		if (QuestRequireData.Contains(QuestRequire->GetQuestId()))
		{
			QuestRequireData[QuestRequire->GetQuestId()].Add(*QuestRequire);
		}
		else
		{
			TArray<FQuestRequireData> NewRequireData;
			NewRequireData.Add(*QuestRequire);

			QuestRequireData.Add(QuestRequire->GetQuestId(), NewRequireData);
		}
	}
}

void UQuestStoryManager::InitializeQuestDialogue()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestStoryDialogueTable(
			TEXT(
				"'/Game/Systems/QuestStory/Data/DT_QuestStoryDialogue.DT_QuestStoryDialogue'"));

	if (!QuestStoryDialogueTable.Succeeded())
	{
		return;
	}

	TArray<FQuestStoryDialogue*> TempQuestStoryDialogueTable;
	QuestStoryDialogueTable.Object->GetAllRows<FQuestStoryDialogue>(
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

			QuestDialogueData.Add(StoryDialogueData->GetQuestId(),
			                      NewTempData);
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
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestSceneCutDataTable(
			TEXT(
				"'/Game/Systems/QuestStory/Data/DT_QuestSceneCutData.DT_QuestSceneCutData'"));

	if (!QuestSceneCutDataTable.Succeeded())
	{
		return;
	}

	TArray<FQuestSceneCutData*> TempQuestSceneCutList;
	QuestSceneCutDataTable.Object->GetAllRows<FQuestSceneCutData>(
		TEXT(""), TempQuestSceneCutList);

	for (const FQuestSceneCutData* TempQuestSceneCutData :
	     TempQuestSceneCutList)
	{
		QuestSceneCutData.Add(TempQuestSceneCutData->GetQuestSceneId(),
		                      *TempQuestSceneCutData);
	}
}

bool UQuestStoryManager::IsHiddenInQuestBook(const FString& QuestId)
{
	if (!QuestData[QuestId].GetQuestMetaData().Contains(EQuestStoryMetaDataKey::IsHideInQuestBook))
	{
		return false;
	}

	return QuestData[QuestId].GetQuestMetaData()[EQuestStoryMetaDataKey::IsHideInQuestBook] == TEXT("true");
}

bool UQuestStoryManager::CheckAndCompleteQuest(AMainPlayerController* PC,
												const FString& QuestId)
{
	if (!PC->HasAuthority())
	{
		return false;
	}

	if (QuestData[QuestId].GetQuestObjective() == EQuestStoryObjective::Dialogue)
	{
		CompleteQuest_Internal(PC, QuestId);
		return true;
	}

	const AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	const UInventoryComponent* InventoryComponent = PS->GetInventoryComponent();
	
	for (FQuestRequireData& RequireData : QuestRequireData[QuestId])
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				FItemMetaInfo RequireItem;
				RequireItem.SetId(RequireData.GetRequireItemOptions().GetRequireItemId());
				RequireItem.SetCurrentCount(RequireData.GetRequireItemOptions().GetRequireItemCount());
				RequireItem.SetMetaData(RequireData.GetRequireItemOptions().GetRequireItemMetaData());
				
				if (!InventoryComponent->GetCurrentRemainItemMetaValue().Contains(RequireItem))
				{
					return false;
				}
				
				if (InventoryComponent->GetCurrentRemainItemMetaValue()[RequireItem]
					< RequireItem.GetCurrentCount())
				{
					return false;
				}
			}
		case EQuestRequireType::HasGold:
			{
				if (PS->GetGold() < RequireData.GetRequireGoldOptions().GetRequireGoldAmount())
				{
					return false;
				}
			}
		default:
			{
				break;
			}
		}
	}

	CompleteQuest_Internal(PC, QuestId);
	
	return true;
}

TArray<FQuestRequireData> UQuestStoryManager::GetRequireQuestDataById(
	const FString& QuestId)
{
	if (!QuestRequireData.Contains(QuestId))
	{
		QuestRequireData.Add(QuestId, TArray<FQuestRequireData>());
	}

	return QuestRequireData[QuestId];
}

uint32 UQuestStoryManager::GetRequireQuestDateToAbleFinish(
	const AMainPlayerController* PC, const FString& QuestId)
{
	uint32 Result = 0;
	if (!QuestRequireData.Contains(QuestId))
	{
		QuestRequireData.Add(QuestId, TArray<FQuestRequireData>());
		return 0;
	}

	for (FQuestRequireData RequireData : QuestRequireData[QuestId])
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				const AMainPlayerState* PS = PC->GetPlayerState<
					AMainPlayerState>();
				
				FItemMetaInfo RequireItem;
				RequireItem.SetId(RequireData.GetRequireItemOptions().GetRequireItemId());
				RequireItem.SetCurrentCount(RequireData.GetRequireItemOptions().GetRequireItemCount());
				RequireItem.SetMetaData(RequireData.GetRequireItemOptions().GetRequireItemMetaData());
				
				if (PS->GetInventoryComponent()->HasExactItemInInventory(RequireItem))
				{
					Result += 1;
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}

	return Result;
}

void UQuestStoryManager::CompleteQuest_Internal(AMainPlayerController* PC,
                                                const FString& QuestId)
{
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	
	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];

	for (FQuestRequireData& RequireData : QuestRequireData[QuestId])
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				FItemMetaInfo RequireItem;
				RequireItem.SetId(RequireData.GetRequireItemOptions().GetRequireItemId());
				RequireItem.SetCurrentCount(RequireData.GetRequireItemOptions().GetRequireItemCount());
				RequireItem.SetMetaData(RequireData.GetRequireItemOptions().GetRequireItemMetaData());
				
				PS->GetInventoryComponent()->RemoveExactItem(RequireItem);
			}
		case EQuestRequireType::HasGold:
			{
				PS->SetGold(PS->GetGold() - RequireData.GetRequireGoldOptions().GetRequireGoldAmount());
			}
		default:
			{
				break;
			}
		}
	}

	if (CurrentQuestStoryData.GetQuestMetaData().Contains(
		EQuestStoryMetaDataKey::NextQuest))
	{
		PC->StartQuest(
			CurrentQuestStoryData.GetQuestMetaData()[
				EQuestStoryMetaDataKey::NextQuest]);
	}

	GiveRewardQuest_Internal(PC, QuestId);
}

void UQuestStoryManager::GiveRewardQuest_Internal(AMainPlayerController* PC,
                                                  const FString& QuestId)
{
	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();

	for (const FQuestRewardData RewardData : CurrentQuestStoryData.
	     GetQuestRewardData())
	{
		switch (RewardData.GetRewardType())
		{
		case EQuestStoryRewardType::Gold:
			{
				// 기존 골드에 값을 추가해준다.
				// 골드는 Id 자체가 Value로 취급된다.
				PS->SetGold(
					PS->GetGold() + FCString::Atoi(*RewardData.GetRewardId()));
			}
		case EQuestStoryRewardType::Item:
			{
			}
		default:
			{
				break;
			}
		}
	}
}
