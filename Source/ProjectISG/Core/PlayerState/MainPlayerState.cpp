#include "MainPlayerState.h"

#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/GAS/Common/Attribute/ISGAttributeSet.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");

	AttributeSet = CreateDefaultSubobject<UISGAttributeSet>(
		TEXT("Attribute Set"));


	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(
		"Inventory Component");
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMainPlayerState::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AGridManager> It(GetWorld()); It; ++It)
	{
		AGridManager* Found = *It;
		if (Found)
		{
			GridManager = Found;
			break;
		}
	}

	if (!GridManager)
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("World에 GridManager가 없습니다. Grid를 사용하는 맵이라면 반드시 World에 배치하세요."), true, true, FLinearColor::Red);
	}
}

void AMainPlayerState::InitializeData()
{
	GetInventoryComponent()->InitializeItemData();
}
