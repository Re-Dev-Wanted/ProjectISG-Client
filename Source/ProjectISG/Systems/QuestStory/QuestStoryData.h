#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "QuestStoryEnum.h"

#include "QuestStoryData.generated.h"

enum class EMetaDataKey : uint32;

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
	GETTER_REF(TArray<FString>, RequireQuestIdList)

	FORCEINLINE TMap<EQuestStoryMetaDataKey, FString>& GetQuestMetaData()
	{
		return QuestMetaData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestTitle;

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, MultiLine = true))
	FString QuestDescription;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestHint;

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, MultiLine = true))
	FText QuestScenario;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestStoryType QuestType = EQuestStoryType::None;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestStoryObjective QuestObjective = EQuestStoryObjective::None;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<EQuestStoryMetaDataKey, FString> QuestMetaData;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<FString> RequireQuestIdList;
};

USTRUCT(BlueprintType)
struct FItemQuestData
{
	GENERATED_BODY()

	GETTER(uint32, ItemId)
	GETTER(uint32, ItemCount)

	FORCEINLINE TMap<EMetaDataKey, FString>& GetItemMetaData()
	{
		return ItemMetaData;
	}

	FORCEINLINE TMap<EQuestAdditiveItemMetaData, FString>&
	GetItemAdditiveMetaData()
	{
		return ItemAdditiveMetaData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 ItemId = 0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 ItemCount = 0;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<EMetaDataKey, FString> ItemMetaData;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<EQuestAdditiveItemMetaData, FString> ItemAdditiveMetaData;
};

USTRUCT(BlueprintType)
struct FGoldQuestData
{
	GENERATED_BODY()

	GETTER(uint32, GoldAmount)

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 GoldAmount = 0;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestRequireData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(EQuestRequireType, RequireType)
	GETTER_REF(FItemQuestData, RequireItemOptions)
	GETTER_REF(FGoldQuestData, RequireGoldOptions)
	GETTER(FString, RequireCustomOptions)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestRequireType RequireType = EQuestRequireType::None;

	UPROPERTY(EditDefaultsOnly
		, meta = (AllowPrivateAccess = true, EditCondition =
			"RequireType == EQuestRequireType::HasItem"))
	FItemQuestData RequireItemOptions = FItemQuestData();

	UPROPERTY(EditDefaultsOnly
		, meta = (AllowPrivateAccess = true, EditCondition =
			"RequireType == EQuestRequireType::HasGold"))
	FGoldQuestData RequireGoldOptions = FGoldQuestData();

	UPROPERTY(EditDefaultsOnly
		, meta = (AllowPrivateAccess = true, EditCondition =
			"RequireType == EQuestRequireType::Custom"))
	FString RequireCustomOptions = FString();
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestRewardData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(EQuestRewardType, RewardType)
	GETTER_REF(FItemQuestData, RewardItemOptions)
	GETTER_REF(FGoldQuestData, RewardGoldOptions)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EQuestRewardType RewardType = EQuestRewardType::None;

	UPROPERTY(EditDefaultsOnly
		, meta = (AllowPrivateAccess = true, EditCondition =
			"RewardType == EQuestRewardType::Item"))
	FItemQuestData RewardItemOptions = FItemQuestData();

	UPROPERTY(EditDefaultsOnly
		, meta = (AllowPrivateAccess = true, EditCondition =
			"RewardType == EQuestRewardType::Gold"))
	FGoldQuestData RewardGoldOptions = FGoldQuestData();
};

// 퀘스트 대사에 대한 데이터
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestStoryDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(uint32, DialogueIndex)
	GETTER(FText, DialogueText)
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
	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, MultiLine = true))
	FText DialogueText;

	// 특정 대사 이후에 특정 예외 동작을 위한 처리로, 각 대사별 특수 모션에 대한 처리 담당
	// UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	// TMap<FString, FString> QuestBehaviorMetaData;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestSceneImageData
{
	GENERATED_USTRUCT_BODY()

	GETTER(TObjectPtr<UTexture2D>, SceneImage)
	GETTER_REF(FText, SceneComment)
	GETTER_REF(FString, DialogueOwner)
	GETTER(float, SceneTime)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTexture2D> SceneImage;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FText SceneComment;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString DialogueOwner;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	float SceneTime = 0.f;
};

// 스토리 진행 상 컷 신이 필요한 경우에 대한 대응 처리
// 이미지 데이터 배열을 저장하고 불러와 이미지를 할당해주는 역할
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestSceneCutData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestSceneId)
	GETTER_REF(TArray<FQuestSceneImageData>, SceneCutList)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestSceneId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<FQuestSceneImageData> SceneCutList;
};
