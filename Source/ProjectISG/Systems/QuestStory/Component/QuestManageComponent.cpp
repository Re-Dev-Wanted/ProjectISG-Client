#include "QuestManageComponent.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Systems/QuestStory/QuestStoryEnum.h"
#include "ProjectISG/Systems/QuestStory/Manager/QuestStoryManager.h"

UQuestManageComponent::UQuestManageComponent()
{
}

void UQuestManageComponent::StartQuest(const FString& NewQuestId)
{
	// 이미 동일한 퀘스트를 수행중이면 수행하지 않는다.
	if (NewQuestId == CurrentPlayingQuestId)
	{
		return;
	}

	// 이미 완료된 퀘스트는 수행하지 않는다.
	if (CompletedQuestIdList.Contains(NewQuestId))
	{
		return;
	}

	CurrentPlayingQuestId = NewQuestId;

	const AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	if (!PC)
	{
		return;
	}

	// MainHud 환경에서만 MainHUD 관련 퀘스트 진행을 수행하도록 설정
	if (PC->GetUIManageComponent()->GetTopStackUI() ==
		EUIName::Gameplay_MainHUD)
	{
		PC->GetMainHUD()->ToggleCurrentQuestUI(true);
		PC->GetMainHUD()->ToggleAutoQuestUI(true);
	}
}

void UQuestManageComponent::StartScene(const FString& NewSceneId)
{
	CurrentPlayingSceneId = NewSceneId;

	AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	if (!PC)
	{
		return;
	}

	const FQuestSceneCutData SceneData =
		UQuestStoryManager::GetQuestSceneCutById(NewSceneId);

	switch (SceneData.GetQuestSceneType())
	{
	case EQuestSceneType::Image:
		{
			PC->PushUI(EUIName::Popup_SceneListUI);
			break;
		}
	case EQuestSceneType::Media:
		{
			PC->PushUI(EUIName::Popup_MediaSceneListUI);
			break;
		}
	default:
		{
			break;
		}
	}
}

bool UQuestManageComponent::EndQuest(const bool IsCheckedQuestEnd)
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	// IsCheckedQuestEnd = true는 보통 서버에서 이루어진다. IsCheckedQuestEnd 자체는 이제
	// 서버에서 조건을 제대로 검증하고 퀘스트를 완료하는 역할을 수행한다.
	if (IsCheckedQuestEnd)
	{
		if (!UQuestStoryManager::CheckAndCompleteQuest(
			PC, CurrentPlayingQuestId))
		{
			return false;
		}
	}

	PC->GetMainHUD()->ToggleAutoQuestUI(false);
	PC->GetMainHUD()->ToggleCurrentQuestUI(false);

	CompletedQuestIdList.Add(CurrentPlayingQuestId);

	// 진행 중인 퀘스트 값 초기화
	CurrentPlayingQuestId = TEXT("");

	return true;
}

EQuestStatus UQuestManageComponent::GetQuestStatusById(
	const FString& QuestId) const
{
	AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	// 이미 완료된 퀘스트인지 검증
	if (CompletedQuestIdList.Contains(QuestId))
	{
		return EQuestStatus::Completed;
	}

	// 현재 해당 퀘스트를 진행 중인지 우선 확인
	if (QuestId == CurrentPlayingQuestId)
	{
		// 완료 가능 여부 탐색
		if (UQuestStoryManager::CheckCompleteQuest(PC, QuestId))
		{
			return EQuestStatus::CanComplete;
		}

		return EQuestStatus::InProgress;
	}

	// 마지막으로 해당 퀘스트를 진행 가능, 불가능 여부를 판단
	for (const FString& RequireQuestIdList :
		UQuestStoryManager::GetQuestDataById(QuestId).GetRequireQuestIdList())
	{
		// 요구 완료 퀘스트 중 하나라도 만족 못하는 경우는 진행 불가능
		if (!CompletedQuestIdList.Contains(RequireQuestIdList))
		{
			return EQuestStatus::Unavailable;
		}
	}

	// 남은 경우 수는 가능 함으로 둔다.
	return EQuestStatus::Available;
}
