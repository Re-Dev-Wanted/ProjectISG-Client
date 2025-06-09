#include "QuestCompleteNotification.h"
#include "Animation/UMGSequencePlayer.h"

void UQuestCompleteNotification::OnNotify()
{
	if (!AppearAnimation)
	{
		return;
	}

	if (AnimationPlayer)
	{
		AnimationPlayer->Stop();
	}
	
	AnimationPlayer = PlayAnimation(AppearAnimation);
}
