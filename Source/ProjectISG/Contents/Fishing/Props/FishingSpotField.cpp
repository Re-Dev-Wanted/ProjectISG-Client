#include "FishingSpotField.h"

#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

AFishingSpotField::AFishingSpotField()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

	BlockCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Block"));
	BlockCollision->SetupAttachment(Root);
	BlockCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BlockCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BlockCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AFishingSpotField::BeginPlay()
{
	Super::BeginPlay();

	ATimeManager* TimeManager = Cast<ATimeManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass()));
	if (TimeManager)
	{
		TimeManager->OnContentRestrictionTimeReached.AddDynamic(this, &ThisClass::ContentRestrictionResponse);
	}
}

bool AFishingSpotField::GetCanTouch() const
{
	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		FString HandItemUsingType = Player->GetHandSlotComponent()->GetItemUsingType();

		return HandItemUsingType == "Fishing";
	}

	return false;
}

FString AFishingSpotField::GetTouchDisplayText(AActor* Causer) const
{
	return TEXT("낚시하기");
}

void AFishingSpotField::OnTouch(AActor* Causer)
{
	IInteractionInterface::OnTouch(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		CurrentPlayer = Player;
		FString HandItemUsingType = Player->GetHandSlotComponent()->GetItemUsingType();

		if (HandItemUsingType == "Fishing")
		{
			FGameplayEventData EventData;
			EventData.EventTag = ISGGameplayTags::Fishing_Active_CastBobber;
			EventData.Instigator = Player;
			EventData.Target = this;
			
			Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_CastBobber, &EventData);
		}
	}
}

void AFishingSpotField::OnTouchResponse(AActor* Causer)
{
	IInteractionInterface::OnTouchResponse(Causer);
}

void AFishingSpotField::ContentRestrictionResponse()
{
	if (CurrentPlayer)
	{
		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Fishing_Active_ReelInLine;
		EventData.Instigator = CurrentPlayer;
			
		CurrentPlayer->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_ReelInLine, &EventData);
	}
	CurrentPlayer = nullptr;
}
