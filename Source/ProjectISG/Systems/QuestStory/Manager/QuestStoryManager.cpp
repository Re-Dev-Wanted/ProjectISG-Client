#include "QuestStoryManager.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/QuestStory/QuestStoryConstants.h"

TArray<FQuestStoryData> UQuestStoryManager::QuestArrayList;
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

bool UQuestStoryManager::CheckAndCompleteQuest(AMainPlayerController* PC,
                                               const FString& QuestId)
{
	if (!PC->HasAuthority())
	{
		return false;
	}

	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];
	bool IsSuccess = false;

	switch (CurrentQuestStoryData.GetQuestObjective())
	{
	case EQuestStoryObjective::Dialogue:
		{
			IsSuccess = true;
			break;
		}
	case EQuestStoryObjective::CollectItem:
		{
			UInventoryComponent* PlayerInventory = PC->GetPlayerState<
				AMainPlayerState>()->GetInventoryComponent();

			TArray<FString> RequireItemTable = GetQuestRequiredItemTableById(
				QuestId);

			bool TempSuccess = true;
			// 정상적으로 아이템이 존재하는 지 검증한다.
			for (FString ItemTable : RequireItemTable)
			{
				TArray<FString> RequireItemData;
				ItemTable.ParseIntoArray(RequireItemData, TEXT(","), true);

				const uint32 ItemRequireId = FCString::Atoi(
					*RequireItemData[0]);
				const uint32 ItemRequireCount = FCString::Atoi(
					*RequireItemData[1]);

				if (PlayerInventory->GetCurrentRemainItemValue()[ItemRequireId]
					< ItemRequireCount)
				{
					TempSuccess = false;
					break;
				}
			}

			// 여기서 이미 실패 판정이 되었으면 뒤로 나간다.
			if (!TempSuccess)
			{
				break;
			}

			// 아이템이 정상적으로 존재하는 경우 RemoveItemToClear 가
			// true 인 상태라면 제거해준다.
			if (CurrentQuestStoryData.GetQuestMetaData().Contains(
					EQuestStoryMetaDataKey::RemoveItemToClear) &&
				CurrentQuestStoryData.
				GetQuestMetaData()[EQuestStoryMetaDataKey::RemoveItemToClear] ==
				"true")
			{
				for (FString ItemTable : RequireItemTable)
				{
					TArray<FString> RequireItemData;
					ItemTable.ParseIntoArray(
						RequireItemData, TEXT(","), true);

					const uint32 ItemRequireId = FCString::Atoi(
						*RequireItemData[0]);
					const uint32 ItemRequireCount = FCString::Atoi(
						*RequireItemData[1]);

					PlayerInventory->
						RemoveItem(ItemRequireId, ItemRequireCount);
				}
			}

			IsSuccess = true;
			break;
		}
	default:
		{
			break;
		}
	}

	if (IsSuccess)
	{
		CompleteQuest_Internal(PC, QuestId);
	}

	return IsSuccess;
}

TArray<FString> UQuestStoryManager::GetQuestRequiredItemTableById(
	const FString& QuestId)
{
	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];
	TArray<FString> RequireItemTable;

	// RequireItem Table이 있는 경우에 대해서 Parsing을 진행한다.
	if (CurrentQuestStoryData.GetQuestMetaData().Contains(
		EQuestStoryMetaDataKey::RequireItem))
	{
		// "/"를 기준으로 아이템 Key와 Value 테이블을 가져온다.
		CurrentQuestStoryData.GetQuestMetaData()[
				EQuestStoryMetaDataKey::RequireItem].
			ParseIntoArray(RequireItemTable, TEXT("/"), true);
	}

	return RequireItemTable;
}

void UQuestStoryManager::CompleteQuest_Internal(AMainPlayerController* PC,
                                                const FString& QuestId)
{
	FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];

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

uint32 UQuestStoryManager::GetQuestAllBehaviorCount(const FString& QuestId)
{
	const FQuestStoryData CurrentQuestStoryData = QuestData[QuestId];
	// 퀘스트 형태가 단순 Dialogue 형태인 경우는 따로 완료해야 할 경우 수가 없어
	// 1개로 설정한다.
	if (CurrentQuestStoryData.GetQuestObjective() ==
		EQuestStoryObjective::Dialogue)
	{
		return 1;
	}

	uint32 Result = 0;
	// 요구 아이템에 따른 갯수
	Result += GetQuestRequiredItemTableById(QuestId).Num();

	return Result;
}
