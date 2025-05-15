#include "LootContainer.h"

#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/UIC_LootContainerUI.h"
#include "ProjectISG/Systems/LootContainer/LootContainerSubsystem.h"

void ALootContainer::BeginPlay()
{
	Super::BeginPlay();
}

void ALootContainer::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALootContainer, Guid);
}

void ALootContainer::SetOption(bool bIsGhost, bool bIsBlock)
{
	Super::SetOption(bIsGhost, bIsBlock);
	
}

void ALootContainer::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);
	
	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(Player->GetController());

		if (!PC)
		{
			return;
		}

		const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

		if (!Guid.IsValid())
		{
			Guid = FGuid::NewGuid();
			PS->GetLootContainerComponent()->CreateLootContainer(Causer, Guid, Capacity);
		}

		TArray<FItemMetaInfo> Items = PS->GetLootContainerComponent()->GetContainerItems(Guid);
		
		if (Player->IsLocallyControlled())
		{
			PC->PushUI(EUIName::Popup_LootContainerUI);

			UUIC_LootContainerUI* UIController = Cast<UUIC_LootContainerUI>
			(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_LootContainerUI]);

			UIController->SetContainer(Guid, Items);
		}
	}
}

bool ALootContainer::GetCanInteractive() const
{
	return true;
}

FString ALootContainer::GetInteractiveDisplayText() const
{
	return TEXT("열기");
}

