#include "Altar.h"

#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/CutSceneList/UIC_SceneListUI.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"


AAltar::AAltar()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAltar::BeginPlay()
{
	Super::BeginPlay();
}

void AAltar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAltar::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);

	// 서버만 엔딩 퀘스트에 접근할 수 있기에 클라이언트가 접근하면 return
	if (Player->HasAuthority() == false)
	{
		return;
	}

	AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();
	AMainPlayerController* PC = Player->GetController<AMainPlayerController>();
	if (Player && PS && PC)
	{
		if (PS->GetInventoryComponent()->HasItemInInventory(OfferingFoodId, 1) &&
			PC->GetQuestManageComponent()->GetCurrentPlayingQuestId() == FString::Printf(TEXT("Story_006")))
		{
			// 공물 음식 삭제
			PS->GetInventoryComponent()->RemoveItem(OfferingFoodId, 1);
			// 퀘스트 완료
			PC->SetCustomQuestComplete(true);

			PC->PushUI(EUIName::Popup_EndingChoice);
		}
	}
}

FString AAltar::GetInteractiveDisplayText() const
{
	return TEXT("제단에 공물 바치기");
}

bool AAltar::GetCanInteractive() const
{
	return true;
}

void AAltar::MoveToLobby()
{
	GetGameInstance<UISGGameInstance>()->DestroySessionAndMoveLevel(LobbyLevel);
}
