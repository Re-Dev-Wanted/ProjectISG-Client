#include "LootContainer.h"

#include "ProjectISG/Systems/LootContainer/LootContainerSubsystem.h"

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
}

bool ALootContainer::GetCanInteractive() const
{
	return true;
}

FString ALootContainer::GetInteractiveDisplayText() const
{
	return TEXT("열기");
}

