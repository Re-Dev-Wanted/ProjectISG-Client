#pragma once

#include "QuestStoryEnum.generated.h"

UENUM()
enum class EQuestStoryType: uint8
{
	None,
	// 인게임 중 메인 HUD 없이 진행되는 스토리로, 시네마틱과 함께 주로 플레이될 수 있다.
	MainQuest,
	// 인게임 중 메인 HUD에 노출되는 스토리로, 주로 부가적인 퀘스트에 사용된다.
	SubQuest,
};

UENUM()
enum class EQuestStoryObjective : uint8
{
	None,
	// 단순 대사 완료 시 퀘스트 자동 완료
	Dialogue,
	// 특정 아이템을 수집해 가야 함.
	CollectItem,
	// 기타
	Custom,
};

UENUM()
enum class EQuestRequireType : uint8
{
	None, HasItem, HasGold, Custom, Interaction,
};

UENUM()
enum class EQuestRewardType : uint8
{
	None, Item, Gold, Custom,
};

UENUM()
enum class EQuestStoryMetaDataKey: uint8
{
	None, IsHideInQuestBook, RemoveItemToClear,
};

UENUM()
enum class EQuestStatus: uint8
{
	None, Available UMETA(DisplayName = "진행 가능") ,
	Unavailable UMETA(DisplayName = "진행 불가능") ,
	InProgress UMETA(DisplayName = "진행 중") ,
	CanComplete UMETA(DisplayName = "완료 가능") ,
	Completed UMETA(DisplayName = "완료됨")};
