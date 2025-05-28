#include "QuestStoryManager.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
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
	
	for (FQuestRequireData& RequireData : GetRequireQuestDataById(QuestId))
	{
		switch (RequireData.GetRequireType())
		{
		case EQuestRequireType::HasItem:
			{
				FItemMetaInfo RequireItem;
				FormattingRequireItem(RequireData, RequireItem);

				// 아이템 비교 로직에 실패한 경우 false를 반환해준다.
				if (!IsExactRequireItemOption(PC, RequireData))
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
			}
		case EQuestRequireType::None:
			{
				return false;
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
				FormattingRequireItem(RequireData, RequireItem);

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
				FormattingRequireItem(RequireData, RequireItem);

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

void UQuestStoryManager::FormattingRequireItem(
	FQuestRequireData& RequireData, FItemMetaInfo& ItemMetaInfo)
{
	// 기본 MetaData 세팅
	ItemMetaInfo.SetId(
		RequireData.GetRequireItemOptions().GetItemId());
	ItemMetaInfo.SetCurrentCount(
		RequireData.GetRequireItemOptions().GetItemCount());
	ItemMetaInfo.SetMetaData(
		RequireData.GetRequireItemOptions().GetItemMetaData());
}

bool UQuestStoryManager::IsExactRequireItemOption(AMainPlayerController* PC,
                                                  FQuestRequireData&
                                                  RequireData)
{
	// Require 타입이 Item 요구가 아니면 굳이 계산할 필요 없다.
	if (RequireData.GetRequireType() != EQuestRequireType::HasItem)
	{
		return false;
	}

	// Require Item Meta Info에 대해 Formatting하고 시작한다.
	FItemMetaInfo RequireItemMetaInfo;
	FormattingRequireItem(RequireData, RequireItemMetaInfo);

	// 부가 데이터를 가져와 해당 데이터 기반으로 분석 기준을 재정립 한다.
	const TMap<EQuestAdditiveItemMetaData, FString> AdditiveData = RequireData.
		GetRequireItemOptions().GetItemAdditiveMetaData();

	const AMainPlayerState* PS = PC->GetPlayerState<AMainPlayerState>();
	const UInventoryComponent* InventoryComponent = PS->GetInventoryComponent();

	// 랭크 무시 관련 태그를 보유한 경우에 대해 처리한다.
	if (AdditiveData.Contains(EQuestAdditiveItemMetaData::IgnoreItemRank))
	{
		// 랭크 무시 시 단순 Id를 기반으로 값이 존재하는지 확인한다.
		if (InventoryComponent->GetCurrentRemainItemValue().Contains(
			RequireItemMetaInfo.GetId()))
		{
			// RequireItemMetaInfo의 요구하는 갯수만큼 Inventory에 보유하고 있는 지 검증한다.
			return InventoryComponent->GetCurrentRemainItemValue()[
				RequireItemMetaInfo.
				GetId()] >= RequireItemMetaInfo.GetCurrentCount();
		}

		// Inventory에 비교할 아이템의 ID 값이 존재하지 않으면 애초부터 비교가 불가능하기에
		// false를 반환한다.
		return false;
	}

	// 위에 조건들을 전부 무시하고 나면, 단순히 아이템을 포함하고 있는지 비교한다..
	if (!InventoryComponent->GetCurrentRemainItemMetaValue().
	                         Contains(RequireItemMetaInfo))
	{
		return false;
	}

	if (InventoryComponent->GetCurrentRemainItemMetaValue()[
		RequireItemMetaInfo] < RequireItemMetaInfo.GetCurrentCount())
	{
		return false;
	}

	return true;
}
