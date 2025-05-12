#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "QuestStoryEnum.h"

#include "QuestStoryData.generated.h"

// 실제 퀘스트 가장 큰 데이터
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestStoryData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(FString, QuestDescription)
	GETTER(EQuestStoryType, QuestType)

	FORCEINLINE TMap<uint32, uint32>& GetRewardItemData() { return RewardItemData; }
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestDescription;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryType QuestType = EQuestStoryType::None;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryObjective QuestObjective = EQuestStoryObjective::None;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<uint32, uint32> RewardItemData;
};

// 퀘스트 대사에 대한 데이터
USTRUCT(BlueprintType)
struct PROJECTISG_API FQuestStoryDialogue : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	GETTER(FString, QuestId)
	GETTER(uint32, DialogueIndex)
	GETTER(FString, DialogueText)
	
private:
	// 대사가 적용되는 퀘스트 Id
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	// 특정 퀘스트에 노출될 대사의 순서
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 DialogueIndex = 0;

	// 대사 텍스트
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString DialogueText;

	// 특정 대사 이후에 특정 예외 동작을 위한 처리로, 각 대사별 특수 모션에 대한 처리 담당
	// UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	// TMap<FString, FString> QuestMetaData;
};
