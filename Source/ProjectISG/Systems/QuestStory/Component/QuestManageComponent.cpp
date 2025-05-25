#include "QuestManageComponent.h"

#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"

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
	if (EndQuestIdList.Contains(NewQuestId))
	{
		return;
	}

	CurrentPlayingQuestId = NewQuestId;

	const AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	if (!PC)
	{
		return;
	}

	PC->GetMainHUD()->StartAutoQuest(NewQuestId);
	PC->GetMainHUD()->ToggleCurrentQuestUI(true);
}

void UQuestManageComponent::StartScene(const FString& NewSceneId)
{
	CurrentPlayingSceneId = NewSceneId;

	AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());

	if (!PC)
	{
		return;
	}

	PC->PushUI(EUIName::Popup_SceneListUI);
}

void UQuestManageComponent::EndQuest(const bool IsSuccess)
{
	const AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());
	if (IsSuccess)
	{
		UE_LOG(LogTemp, Display,
		       TEXT("Quest Manage Component: 여기에 퀘스트 완료 시 보상 처리"))
		PC->GetMainHUD()->ToggleAutoQuestUI(false);
		EndQuestIdList.Add(CurrentPlayingQuestId);
	}
}
