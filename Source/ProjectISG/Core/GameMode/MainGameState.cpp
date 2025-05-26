#include "MainGameState.h"

#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

AMainGameState::AMainGameState()
{
}

void AMainGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainGameState, CurrentWorldQuestId)
	DOREPLIFETIME(AMainGameState, SessionId)
}

void AMainGameState::StartWorldQuest(const FString& QuestId)
{
	CurrentWorldQuestId = QuestId;

	for (const TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerState->GetPlayerController());

		// 서버에서만 호출되는 이 함수가 LocalController인 경우는
		// 무조건 서버 Controller이고 나머지가 Client Controller라는 반증
		if (PC->IsLocalController())
		{
			PC->StartQuestToPlayer(QuestId);
		}
		else
		{
			PC->Client_StartQuestToPlayer(QuestId);
		}
	}
}

void AMainGameState::EndWorldQuest()
{
	if (!HasAuthority())
	{
		return;
	}

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		UGameplayStatics::GetPlayerController(GetWorld(), 0));
	// 서버에서 퀘스트 종료가 확인되는 경우, 다른 모든 유저도 현재 퀘스트를 종료한다.
	// 이것은 퀘스트 자체가 서로 공유되기 때문에 가능한 일이다.
	if (PC->GetQuestManageComponent()->EndQuest(true))
	{
		for (const TObjectPtr<APlayerState> PlayerState : GetWorld()->
			GetGameState()->PlayerArray)
		{
			// 자기 자신만 제외하기.
			if (!PlayerState->GetOwningController()->IsLocalController())
			{
				Cast<AMainPlayerController>(PlayerState->GetOwningController())
					->Client_EndQuestToPlayer();
			}
		}
	}
}
