#include "Workbench.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"

struct FItemMetaInfo;
class UUIC_LootContainerUI;

void AWorkbench::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWorkbench::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			Player->GetController());

		if (!PC)
		{
			return;
		}

		SetInteractingPlayer(Player);
		
		if (HasAuthority())
		{
			SetOwner(PC);
		}
		else
		{
			PC->Server_SetOwnerActor(this);

			Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
		}

		if (Player->IsLocallyControlled())
		{
			PC->PushUI(EUIName::Popup_CraftingUI);

			UUIC_WorkbenchUI* UIController = Cast<UUIC_WorkbenchUI>
			(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_CraftingUI]);

			UIController->SetUIHandler(this);
		}
		
	}
}

void AWorkbench::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	Server_SetInteractingPlayer(Player);
}

bool AWorkbench::GetCanInteractive() const
{
	return !IsValid(GetInteractingPlayer());
}

bool AWorkbench::GetCanTouch() const
{
	return false;
}

FString AWorkbench::GetInteractiveDisplayText() const
{
	return TEXT("제작하기");
}

void AWorkbench::OnClosed()
{
	if (!IsValid(GetInteractingPlayer()))
	{
		return;
	}
	
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetInteractingPlayer()->GetController());

	if (!PC)
	{
		return;
	}
	
	if (!HasAuthority())
	{
		GetInteractingPlayer()->GetInteractionComponent()->Server_OnInteractiveResponse
		(nullptr);
	}

	SetInteractingPlayer(nullptr);
}

