﻿#include "MainGameState.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

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
