#include "LootContainer.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/UIC_LootContainerUI.h"
#include "ProjectISG/Systems/LootContainer/LootContainerSubsystem.h"

void ALootContainer::BeginPlay()
{
	Super::BeginPlay();

	Guid = GetWorld()->GetGameInstance()
		->GetSubsystem<ULootContainerSubsystem>()
		->CreateLootContainer();
}

void ALootContainer::SetOption(bool bIsGhost, bool bIsBlock)
{
	Super::SetOption(bIsGhost, bIsBlock);

	if (!bIsGhost)
	{
		Guid = GetWorld()->GetGameInstance()
		->GetSubsystem<ULootContainerSubsystem>()
		->CreateLootContainer();
	}
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

		if (Player->IsLocallyControlled())
		{
			PC->PushUI(EUIName::Popup_LootContainerUI);

			UUIC_LootContainerUI* UIController = Cast<UUIC_LootContainerUI>
			(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_LootContainerUI]);

			UIController->SetUI(Guid, Capacity);
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

