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
	GETTER(FString, QuestTitle)
	GETTER(FString, QuestDescription)
	GETTER(FText, QuestScenario)
	GETTER(EQuestStoryType, QuestType)
	GETTER(EQuestStoryObjective, QuestObjective)

	FORCEINLINE TMap<FString, FString>& GetQuestMetaData()
	{
		return QuestMetaData;
	}

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestId;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestTitle;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString QuestDescription;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FText QuestScenario;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryType QuestType = EQuestStoryType::None;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	EQuestStoryObjective QuestObjective = EQuestStoryObjective::None;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	TMap<FString, FString> QuestMetaData;
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
