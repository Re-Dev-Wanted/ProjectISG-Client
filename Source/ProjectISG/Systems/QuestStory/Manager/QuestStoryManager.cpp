#include "QuestStoryManager.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

TArray<FQuestStoryData> UQuestStoryManager::QuestArrayList;
TMap<FString, FQuestStoryData> UQuestStoryManager::QuestData;
TMap<FString, TArray<FQuestRequireData>> UQuestStoryManager::QuestRequireData;
TMap<FString, TArray<FQuestRewardData>> UQuestStoryManager::QuestRewardData;
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
		InitializeQuestRewardData();
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
	if (!QuestDialogueData.Contains(QuestId))
	{
		QuestDialogueData.Add(QuestId, TArray<FQuestStoryDialogue>());
	}

	return *QuestDialogueData.Find(QuestId);
}

FQuestSceneCutData& UQuestStoryManager::GetQuestSceneCutById(
	const FString& SceneId)
{
	if (!QuestSceneCutData.Contains(SceneId))
	{
		QuestSceneCutData.Add(SceneId, FQuestSceneCutData());
	}

	return *QuestSceneCutData.Find(SceneId);
}

void UQuestStoryManager::InitializeQuestData()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestStoryDataTable(TEXT(
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
		QuestRequireDataTable(TEXT(
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

void UQuestStoryManager::InitializeQuestRewardData()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestRewardDataTable(TEXT(
			"'/Game/Systems/QuestStory/Data/DT_QuestRewardData.DT_QuestRewardData'"));

	if (!QuestRewardDataTable.Succeeded())
	{
		return;
	}

	TArray<FQuestRewardData*> TempQuestRewardData;
	QuestRewardDataTable.Object->GetAllRows<FQuestRewardData>(
		TEXT(""), TempQuestRewardData);

	for (const FQuestRewardData* QuestReward : TempQuestRewardData)
	{
		if (QuestRewardData.Contains(QuestReward->GetQuestId()))
		{
			QuestRewardData[QuestReward->GetQuestId()].Add(*QuestReward);
		}
		else
		{
			TArray<FQuestRewardData> NewRewardData;
			NewRewardData.Add(*QuestReward);

			QuestRewardData.Add(QuestReward->GetQuestId(), NewRewardData);
		}
	}
}

void UQuestStoryManager::InitializeQuestDialogue()
{
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestStoryDialogueTable(TEXT(
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
	const static ConstructorHelpers::FObjectFinder<UDataTable>
		QuestSceneCutDataTable(TEXT(
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
		QuestSceneCutData.Add(TempQuestSceneCutData->GetQuestSceneId()
		                      , *TempQuestSceneCutData);
	}
}

bool UQuestStoryManager::IsHiddenInQuestBook(const FString& QuestId)
{
	if (!QuestData[QuestId].GetQuestMetaData().Contains(
		EQuestStoryMetaDataKey::IsHideInQuestBook))
	{
		return false;
	}

	return QuestData[QuestId].GetQuestMetaData()[
		EQuestStoryMetaDataKey::IsHideInQuestBook] == TEXT("true");
}

bool UQuestStoryManager::CheckCompleteQuest(AMainPlayerController* PC
                                            , const FString& QuestId)
{
	if (!PC->HasAuthority())
	{
		return false;
	}

	if (QuestData[QuestId].GetQuestObjective() ==
		EQuestStoryObjective::Dialogue)
	{
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
				RequireItem.SetId(
					RequireData.GetRequireItemOptions().GetItemId());
				RequireItem.SetCurrentCount(
					RequireData.GetRequireItemOptions().GetItemCount());
				RequireItem.SetMetaData(
					RequireData.GetRequireItemOptions().GetItemMetaData());

				if (!InventoryComponent->GetCurrentRemainItemMetaValue().
				                         Contains(RequireItem))
				{
					return false;
				}

				if (InventoryComponent->GetCurrentRemainItemMetaValue()[
					RequireItem] < RequireItem.GetCurrentCount())
				{
					return false;
				}

				break;
			}
		case EQuestRequireType::HasGold:
			{
				if (PS->GetGold() < RequireData.GetRequireGoldOptions().
				                                GetGoldAmount())
				{
					return false;
				}

				break;
			}
		case EQuestRequireType::Custom:
			{
				return PC->GetCustomQuestComplete();
				break;
			}
		default:
			{
				break;
			}
		}
	}

	return true;
}

bool UQuestStoryManager::CheckAndCompleteCustomQuest(
	AMainPlayerController* PC, const FString& QuestId)
{
	if (!PC->HasAuthority())
	{
		return false;
	}

	if (QuestData[QuestId].GetQuestObjective() == EQuestStoryObjective::Custom)
	{
		CompleteQuest_Internal(PC, QuestId);
		return true;
	}

	return false;
}

bool UQuestStoryManager::CheckAndCompleteDialogueQuest(AMainPlayerController* PC
	, const FString& QuestId)
{
	if (!PC->HasAuthority())
	{
		return false;
	}

	if (QuestData[QuestId].GetQuestObjective() ==
		EQuestStoryObjective::Dialogue)
	{
		CompleteQuest_Internal(PC, QuestId);
		return true;
	}

	return false;
}

bool UQuestStoryManager::CheckAndCompleteQuest(AMainPlayerController* PC
                                               , const FString& QuestId)
{
	if (CheckCompleteQuest(PC, QuestId))
	{
		CompleteQuest_Internal(PC, QuestId);

		return true;
	}

	return false;
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

TArray<FQuestRewardData> UQuestStoryManager::GetRewardQuestDataById(
	const FString& QuestId)
{
	if (!QuestRewardData.Contains(QuestId))
	{
		QuestRewardData.Add(QuestId, TArray<FQuestRewardData>());
	}

	return QuestRewardData[QuestId];
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

	const AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();

	for (FQuestRequireData RequireData : QuestRequireData[QuestId])
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				FItemMetaInfo RequireItem;
				RequireItem.SetId(
					RequireData.GetRequireItemOptions().GetItemId());
				RequireItem.SetCurrentCount(
					RequireData.GetRequireItemOptions().GetItemCount());
				RequireItem.SetMetaData(
					RequireData.GetRequireItemOptions().GetItemMetaData());

				if (PS->GetInventoryComponent()->HasExactItemInInventory(
					RequireItem))
				{
					Result += 1;
				}

				break;
			}
		case EQuestRequireType::HasGold:
			{
				if (PS->GetGold() >= RequireData.GetRequireGoldOptions().
				                                 GetGoldAmount())
				{
					Result += 1;
				}

				break;
			}
		case EQuestRequireType::Custom:
			{
				// GetCustomQuestComplete 는 bool이라 참이면 1 더해준다.
				Result += PC->GetCustomQuestComplete();
			}
		default:
			{
				break;
			}
		}
	}

	return Result;
}

void UQuestStoryManager::CompleteQuest_Internal(AMainPlayerController* PC
                                                , const FString& QuestId)
{
	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();

	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];

	// 퀘스트에 필요한 조건 회수 조치 수행
	for (FQuestRequireData& RequireData : GetRequireQuestDataById(QuestId))
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				if (CurrentQuestStoryData.GetQuestMetaData().
				                          Contains(
					                          EQuestStoryMetaDataKey::RemoveItemToClear)
					&& CurrentQuestStoryData.GetQuestMetaData()[
						EQuestStoryMetaDataKey::RemoveItemToClear] == TEXT(
						"false"))
				{
					break;
				}

				FItemMetaInfo RequireItem;
				RequireItem.SetId(
					RequireData.GetRequireItemOptions().GetItemId());
				RequireItem.SetCurrentCount(
					RequireData.GetRequireItemOptions().GetItemCount());
				RequireItem.SetMetaData(
					RequireData.GetRequireItemOptions().GetItemMetaData());

				PS->GetInventoryComponent()->RemoveExactItem(RequireItem);

				break;
			}
		case EQuestRequireType::HasGold:
			{
				PS->SetGold(
					PS->GetGold() - RequireData.GetRequireGoldOptions().
					                            GetGoldAmount());
				break;
			}
		case EQuestRequireType::Custom:
			{
				PC->SetCustomQuestComplete(false);
				break;
			}
		default:
			{
				break;
			}
		}
	}

	// 다음 퀘스트를 수행한다.
	if (CurrentQuestStoryData.GetQuestMetaData().Contains(
		EQuestStoryMetaDataKey::NextQuest))
	{
		PC->StartQuest(
			CurrentQuestStoryData.GetQuestMetaData()[
				EQuestStoryMetaDataKey::NextQuest]);
	}

	// 보상 제공
	GiveRewardQuest_Internal(PC, QuestId);
}

void UQuestStoryManager::GiveRewardQuest_Internal(
	AMainPlayerController* PC, const FString& QuestId)
{
	if (!QuestRewardData.Contains(QuestId))
	{
		return;
	}

	AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();

	for (FQuestRewardData& RewardData : QuestRewardData[QuestId])
	{
		switch (RewardData.GetRewardType())
		{
		case EQuestRewardType::Gold:
			{
				PS->SetGold(
					PS->GetGold() + RewardData.GetRewardGoldOptions().
					                           GetGoldAmount());

				break;
			}
		case EQuestRewardType::Item:
			{
				FItemMetaInfo NewItem;
				NewItem.SetId(RewardData.GetRewardItemOptions().GetItemId());
				NewItem.SetCurrentCount(
					RewardData.GetRewardItemOptions().GetItemCount());
				NewItem.SetMetaData(
					RewardData.GetRewardItemOptions().GetItemMetaData());

				PS->GetInventoryComponent()->AddItem(NewItem);

				break;
			}
		default:
			{
				break;
			}
		}
	}
}
