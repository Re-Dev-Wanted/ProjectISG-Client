#include "QuestManageComponent.h"

// #include "ProjectISG/Core/Controller/MainPlayerController.h"

UQuestManageComponent::UQuestManageComponent()
{
}

void UQuestManageComponent::StartQuest(const FString& NewQuestId)
{
	CurrentPlayingQuestId = NewQuestId;

	// AMainPlayerController* PC = Cast<AMainPlayerController>(GetOwner());
	//
	// if (!PC)
	// {
	// 	return;
	// }
}
