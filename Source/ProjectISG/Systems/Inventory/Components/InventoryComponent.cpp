#include "InventoryComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

UInventoryComponent::UInventoryComponent()
{
}

void UInventoryComponent::InitializeItemData()
{
	InventoryList.SetNum(InventorySlotCount);

	FItemMetaInfo NewItemMetaInfo;
	NewItemMetaInfo.SetId(1);
	NewItemMetaInfo.SetCurrentCount(1);
	InventoryList[0] = NewItemMetaInfo;

	FItemMetaInfo NewItemMetaInfo2;
	NewItemMetaInfo2.SetId(1);
	NewItemMetaInfo2.SetCurrentCount(2);

	InventoryList[1] = NewItemMetaInfo2;

	FItemMetaInfo BuildItemMetaInfo;
	BuildItemMetaInfo.SetId(2);
	BuildItemMetaInfo.SetCurrentCount(1);

	InventoryList[2] = BuildItemMetaInfo;
	AddItemToInventory(2, BuildItemMetaInfo);

	FItemMetaInfo CropItemMetaInfo;
	CropItemMetaInfo.SetId(3);
	CropItemMetaInfo.SetCurrentCount(2);

	InventoryList[3] = CropItemMetaInfo;
	AddItemToInventory(3, CropItemMetaInfo);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UInventoryComponent::HasItemInInventory(const uint32 Id,
                                             const uint32 Count)
{
	if (CurrentRemainItemValue.Find(Id))
	{
		return CurrentRemainItemValue[Id] >= Count;
	}

	return false;
}

// TODO: 이건 내부 로직에서 아이템 변경될 때 마다 처리하기
void UInventoryComponent::UpdateCurrentRemainItemValue()
{
	TMap<uint32, uint32> NewMap;

	for (int i = 0; i < InventoryList.Num(); i++)
	{
		if (NewMap.Find(InventoryList[i].GetId()))
		{
			NewMap[InventoryList[i].GetId()] += InventoryList[i].
				GetCurrentCount();
		}
		else
		{
			NewMap.Add(InventoryList[i].GetId(),
			           InventoryList[i].GetCurrentCount());
		}
	}
	CurrentRemainItemValue.Empty();
	CurrentRemainItemValue.Append(NewMap);
}

void UInventoryComponent::SwapItemInInventory(const uint16 Prev,
                                              const uint16 Next)
{
	const FItemMetaInfo Temp = InventoryList[Prev];
	InventoryList[Prev] = InventoryList[Next];
	InventoryList[Next] = Temp;
}

// 특정 Index에 특정 아이템을 넣어둔다.
uint32 UInventoryComponent::AddItemToInventory(const uint16 Index,
                                               const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = UItemManager::GetItemInfoById(
		ItemInfo.GetId());

	// CurrentItemCount는 우선은 총 합으로 가지고 있는 아이템 수를 의미한다.
	const uint16 CurrentItemCount = InventoryList[Index].GetCurrentCount()
		+
		ItemInfo.GetCurrentCount();

	const uint16 NextSetMainItemCount = UKismetMathLibrary::Min(
		CurrentItemCount
		, ItemInfoById.GetMaxItemCount());

	// 우선 최대 갯수를 차지하면 최대 갯수를 넣고, 그게 아니면 그냥 더한 갯수를 넣어줌
	if (InventoryList[Index].GetId() == 0)
	{
		InventoryList[Index].SetId(ItemInfo.GetId());
		InventoryList[Index].SetMetaData(ItemInfo.GetMetaData());
	}
	InventoryList[Index].SetCurrentCount(NextSetMainItemCount);

	// 최대 값 만큼 넣어도 남는 경우가 존재한다.
	int32 RemainCount = CurrentItemCount - ItemInfoById.GetMaxItemCount();

	// 이 경우는 아이템에 여분이 있어 더 추가해야 하는 경우를 의미한다.
	if (RemainCount > 0)
	{
		// 이미 꽉 찬 경우여도 여분의 칸 검색을 위해
		// 처음부터 다시 탐색해서 남은 값 들을 순차적으로 넣어준다.
		for (int i = 0; i < GetInventorySlotCount(); i++)
		{
			if (RemainCount == 0)
			{
				break;
			}
			// 단순히 i번째에 값이 없는 경우에 대해서는 값을 넣어준다.
			if (InventoryList[i].GetId() == 0)
			{
				FItemMetaInfo NewItemInfo;
				NewItemInfo.SetId(ItemInfo.GetId());
				NewItemInfo.SetMetaData(ItemInfo.GetMetaData());
				NewItemInfo.SetCurrentCount(UKismetMathLibrary::Min(
					RemainCount, ItemInfoById.GetMaxItemCount()));

				InventoryList[i] = NewItemInfo;
				RemainCount -= UKismetMathLibrary::Min(
					RemainCount, ItemInfoById.GetMaxItemCount());
			}
		}
	}

	OnUpdateInventory();
	return RemainCount > 0 ? RemainCount : 0;
}

bool UInventoryComponent::DropItem(const uint16 Index, const uint32 Count)
{
	// -1은 강제로 다 버리기
	if (Count == -1)
	{
		const FItemMetaInfo ClearItemMeta;
		InventoryList[Index] = ClearItemMeta;

		OnUpdateInventory();
		return true;
	}

	if (InventoryList[Index].GetCurrentCount() - Count < 0)
	{
		return false;
	}

	if (InventoryList[Index].GetCurrentCount() - Count == 0)
	{
		const FItemMetaInfo ClearItemMeta;
		InventoryList[Index] = ClearItemMeta;
	}
	else
	{
		InventoryList[Index].SetCurrentCount(
			InventoryList[Index].GetCurrentCount() - Count);
	}

	OnUpdateInventory();

	return true;
}

// 정해진 규칙에 의거해 아이템을 순서대로 넣어둔다.
uint32 UInventoryComponent::AddItem(const FItemMetaInfo& ItemInfo)
{
	const FItemInfoData& ItemInfoById = UItemManager::GetItemInfoById(
		ItemInfo.GetId());

	// 핫바에서 먼저 검색함.
	bool bHasInventory = false;
	// 맨 처음 값은 현재 주운 아이템의 갯수로 지정한다.
	uint32 RemainResult = ItemInfo.GetCurrentCount();

	for (int i = 0; i < GetInventorySlotCount(); i++)
	{
		if (InventoryList[i].GetId() == ItemInfo.GetId()
			&& InventoryList[i].GetCurrentCount() < ItemInfoById.
			GetMaxItemCount())
		{
			bHasInventory = true;
			RemainResult = AddItemToInventory(i, ItemInfo);
			break;
		}
	}

	if (!bHasInventory && RemainResult > 0)
	{
		for (int i = 0; i < GetInventorySlotCount(); i++)
		{
			if (InventoryList[i].GetId() == 0)
			{
				RemainResult = AddItemToInventory(i, ItemInfo);
				break;
			}
		}
	}

	if (RemainResult > 0)
	{
		// TODO: 이후에 대한 처리 로직은 다른 곳에 이관해야 함
		UE_LOG(LogTemp, Error, TEXT("인벤토리 초과함"))
		return RemainResult;
	}

	return 0;
}

bool UInventoryComponent::RemoveItem(const uint16 Id, const uint32 Count)
{
	uint32 RemainNum = Count;
	TArray<uint32> CanRemoveIndexList;

	for (int i = 0; i < GetInventorySlotCount(); i++)
	{
		if (InventoryList[i].GetId() == Id)
		{
			CanRemoveIndexList.Add(i);
			RemainNum = UKismetMathLibrary::Max(
				RemainNum - InventoryList[i].GetCurrentCount(), 0);
		}

		// RemainNum이 0보다 작거나 같다는 의미는 즉
		// 더이상 탐색하지 않아도 전부 없앨 수 있다라는 의미다.
		if (RemainNum <= 0)
		{
			break;
		}
	}

	// 0보다 큰 상황이라면 사용하기에는 양이 부족하다라는 의미기에 false를 반환한다.
	if (RemainNum > 0)
	{
		return false;
	}

	// 다시 RemainNum을 돌려둔다. 재사용해서 갯수를 없애는 것에 실질적으로 이용함
	RemainNum = Count;
	for (const uint32 RemoveIndex : CanRemoveIndexList)
	{
		// 둘 중 더 작은 값을 없애준다. 만약 RemainNum이 10이고, 아이템의 현재 갯수가 7이라면
		// 7개만 없애주고, 아이템 갯수가 13개면 10개만 없앤다.
		const uint32 RemoveCount = UKismetMathLibrary::Min(
			InventoryList[RemoveIndex].GetCurrentCount(), RemainNum);

		// 우선은 값 설정
		InventoryList[RemoveIndex].SetCurrentCount(
			InventoryList[RemoveIndex].GetCurrentCount() - RemoveCount);
		RemainNum -= RemoveCount;

		// 만약 슬롯이 현재 0개라면 슬롯 초기화를 진행한다.
		if (InventoryList[RemoveIndex].GetCurrentCount() == 0)
		{
			const FItemMetaInfo ClearItemMeta;
			InventoryList[RemoveIndex] = ClearItemMeta;
		}

		// 어차피 여기서 다 버려서 0이 될 수 밖에 없다.
		if (RemainNum == 0)
		{
			OnUpdateInventory();
			return true;
		}
	}

	// 이건 혹시 모르는 예외 사항에 대한 처리
	return false;
}

// TODO: 이 함수가 여러번 호출될 수 있음. 차라리 모든 인벤 업데이트 이후 별도로 처리하는 것 또한 좋아보임
void UInventoryComponent::OnUpdateInventory()
{
	UpdateCurrentRemainItemValue();
	OnInventoryUpdateNotified.Broadcast();
}
