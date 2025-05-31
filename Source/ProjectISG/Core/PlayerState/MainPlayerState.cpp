#include "MainPlayerState.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/GAS/Common/ISGAbilitySystemComponent.h"
#include "ProjectISG/GAS/Common/Attribute/ISGAttributeSet.h"
#include "ProjectISG/Systems/Animation/AnimationData.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

AMainPlayerState::AMainPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UISGAbilitySystemComponent>(
		"Ability System Component");

	AttributeSet = CreateDefaultSubobject<UISGAttributeSet>(
		TEXT("Attribute Set"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(
		"Inventory Component");

	CharacterName = ECharacterName::Lumine;
}

UAbilitySystemComponent* AMainPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

bool AMainPlayerState::CanBuyProduct(uint32 ProductPrice)
{
	if (Gold >= ProductPrice)
	{
		return true;
	}
	return false;
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
		UKismetSystemLibrary::PrintString(GetWorld(),
		                                  TEXT(
			                                  "World에 GridManager가 없습니다. Grid를 사용하는 맵이라면 반드시 World에 배치하세요."),
		                                  true, true, FLinearColor::Red);
	}

	TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));
	if (!TimeManager)
	{
		UKismetSystemLibrary::PrintString(GetWorld(),
		                                  TEXT(
			                                  "World에 TimeManager가 없습니다. 시간을 사용하는 맵이라면 반드시 World에 배치하세요."),
		                                  true, true, FLinearColor::Red);
	}
}

void AMainPlayerState::InitializeData()
{
	GetInventoryComponent()->InitializeItemData();
}
