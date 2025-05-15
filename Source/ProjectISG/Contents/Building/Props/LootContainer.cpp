#include "LootContainer.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/LootContainer/UI/UIC_LootContainerUI.h"
#include "ProjectISG/Systems/LootContainer/LootContainerSubsystem.h"

void ALootContainer::BeginPlay()
{
	Super::BeginPlay();

	//TODO: 테스트용 추후에 지우기
	Guid = GetWorld()->GetGameState<AMainGameState>()
		->GetLootContainerComponent()
		->CreateLootContainer(Capacity);
}

void ALootContainer::SetOption(bool bIsGhost, bool bIsBlock)
{
	Super::SetOption(bIsGhost, bIsBlock);

	if (!bIsGhost)
	{
		Guid = GetWorld()->GetGameState<AMainGameState>()->GetLootContainerComponent()
		->CreateLootContainer(Capacity);
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

			TArray<FItemMetaInfo> Items = GetWorld()->GetGameState<AMainGameState>()->
			GetLootContainerComponent()->GetContainerItems(Guid);

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

