#include "Workbench.h"

#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

struct FItemMetaInfo;
class UUIC_LootContainerUI;

void AWorkbench::BeginPlay()
{
	Super::BeginPlay();

	ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
										  ATimeManager::StaticClass()));

	if (TimeManager)
	{
		TimeManager->OnContentRestrictionTimeReached.AddDynamic(
			this, &ThisClass::UnlockPlayer);
	}
	
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

			Player->Server_SetActorTransformReplicated(InteractStartPoint->GetComponentTransform());
		}
		else
		{
			PC->Server_SetOwnerActor(this);

			Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
		}

		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Crafting_Active_StartCrafting;
		EventData.Instigator = Player;
		EventData.Target = this;
		
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void AWorkbench::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	Server_SetInteractingPlayer(Player);

	Player->Server_SetActorTransformReplicated(InteractStartPoint->GetComponentTransform());
}

void AWorkbench::NetMulticast_SetInteractingPlayerPosition_Implementation(AMainPlayerCharacter* Player)
{
	Player->SetActorLocation(InteractStartPoint->GetComponentLocation());
	Player->SetActorRotation(InteractStartPoint->GetComponentRotation());
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

void AWorkbench::UnlockPlayer()
{
	Client_UnlockPlayer();

	if (HasAuthority())
	{
		SetInteractingPlayer(nullptr);
	}
}

void AWorkbench::Client_UnlockPlayer_Implementation()
{
	if (GetInteractingPlayer() == nullptr)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.EventTag = ISGGameplayTags::Crafting_Active_EndCrafting;
	EventData.Instigator = GetInteractingPlayer();
	EventData.Target = this;
		
	GetInteractingPlayer()->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
}

