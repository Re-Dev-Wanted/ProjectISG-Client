#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "QuestStoryEnum.h"

#include "QuestStoryData.generated.h"


enum class EMetaDataKey : uint32;

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestRewardData
{
	GENERATED_USTRUCT_BODY()

	GETTER(EQuestStoryRewardType, RewardType)
	GETTER(FString, RewardId)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestStoryRewardType RewardType = EQuestStoryRewardType::None;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString RewardId;
};

// 실제 퀘스트 가장 큰 데이터
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestStoryData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(FString, QuestTitle)
	GETTER(FString, QuestHint)
	GETTER(FString, QuestDescription)
	GETTER(FText, QuestScenario)
	GETTER(EQuestStoryType, QuestType)
	GETTER(EQuestStoryObjective, QuestObjective)

	FORCEINLINE TMap<EQuestStoryMetaDataKey, FString>& GetQuestMetaData()
	{
		return QuestMetaData;
	}

	FORCEINLINE TArray<FQuestRewardData>& GetQuestRewardData()
	{
		return QuestRewardData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestTitle;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestDescription;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestHint;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FText QuestScenario;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryType QuestType = EQuestStoryType::None;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryObjective QuestObjective = EQuestStoryObjective::None;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<EQuestStoryMetaDataKey, FString> QuestMetaData;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TArray<FQuestRewardData> QuestRewardData;
};

USTRUCT(BlueprintType)
struct FHasItemQuestData
{
	GENERATED_BODY()

	GETTER(uint32, RequireItemId)
	GETTER(uint32, RequireItemCount)

	FORCEINLINE TMap<EMetaDataKey, FString>& GetRequireItemMetaData()
	{
		return RequireItemMetaData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 RequireItemId = 0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 RequireItemCount = 0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<EMetaDataKey, FString> RequireItemMetaData;
};

USTRUCT(BlueprintType)
struct FHasGoldQuestData
{
	GENERATED_BODY()

	GETTER(uint32, RequireGoldAmount)

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 RequireGoldAmount = 0;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestRequireData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(EQuestRequireType, RequireType)
	GETTER_REF(FHasItemQuestData, RequireItemOptions)
	GETTER_REF(FHasGoldQuestData, RequireGoldOptions)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestRequireType RequireType = EQuestRequireType::None;

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, EditCondition =
			"RequireType == EQuestRequireType::HasItem"))
	FHasItemQuestData RequireItemOptions = FHasItemQuestData();

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, EditCondition =
			"RequireType == EQuestRequireType::HasGold"))
	FHasGoldQuestData RequireGoldOptions = FHasGoldQuestData();
};

// 퀘스트 대사에 대한 데이터
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestStoryDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(uint32, DialogueIndex)
	GETTER(FString, DialogueText)
	GETTER(FString, DialogueOwner)

private:
	// 대사가 적용되는 퀘스트 Id
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	// 특정 퀘스트에 노출될 대사의 순서
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 DialogueIndex = 0;

	// 대화하는 주체에 대한 설정
	// {Player}로 지정할 시 플레이어 이름으로 노출 처리
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString DialogueOwner;

	// 대사 텍스트
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString DialogueText;

	// 특정 대사 이후에 특정 예외 동작을 위한 처리로, 각 대사별 특수 모션에 대한 처리 담당
	// UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	// TMap<FString, FString> QuestBehaviorMetaData;
};

// 스토리 진행 상 컷 신이 필요한 경우에 대한 대응 처리
// 이미지 데이터 배열을 저장하고 불러와 이미지를 할당해주는 역할
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestSceneCutData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestSceneId)
	TArray<TObjectPtr<UTexture2D>>& GetSceneImages() { return SceneImages; }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestSceneId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UTexture2D>> SceneImages;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestRewardTemplate : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString RewardId;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestItemReward : public FQuestRewardTemplate
{
	GENERATED_USTRUCT_BODY()

	GETTER(uint32, Count)

	FORCEINLINE TMap<EMetaDataKey, FString>& GetItemMetaData()
	{
		return ItemMetaData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 Count = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<EMetaDataKey, FString> ItemMetaData;
};
