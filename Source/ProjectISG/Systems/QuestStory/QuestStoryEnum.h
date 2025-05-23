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
	CollectItem
};

UENUM()
enum class EQuestRequireType : uint8
{
	None,
	HasItem,
	HasGold,
	Custom,
};

UENUM()
enum class EQuestStoryMetaDataKey: uint8
{
	None,
	RequireItem,
	RemoveItemToClear,
	NextQuest,
};

UENUM()
enum class EQuestStoryRewardType: uint8
{
	None,
	Item,
	Gold,
};
