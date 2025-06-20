﻿#include "KitchenFurniture.h"

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

AKitchenFurniture::AKitchenFurniture()
{
	bReplicates = true;
	Super::SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	KitchenStandPosition = CreateDefaultSubobject<USceneComponent>(
		"Kitchen Stand Position");
	KitchenStandPosition->SetupAttachment(GetRootComponent());

	KitchenCameraComponent = CreateDefaultSubobject<UCameraComponent>(
		"Kitchen Camera Component");
	KitchenCameraComponent->SetupAttachment(GetRootComponent());

	FryPanMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Fry Pan Mesh");
	FryPanMesh->SetupAttachment(GetRootComponent());

	ScoopMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Scoop Mesh");
	ScoopMesh->SetupAttachment(GetRootComponent());

	WokMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Wok Mesh");
	WokMesh->SetupAttachment(GetRootComponent());

	BigPotMesh = CreateDefaultSubobject<UStaticMeshComponent>("Big Pot Mesh");
	BigPotMesh->SetupAttachment(GetRootComponent());

	KitchenFire = CreateDefaultSubobject<UNiagaraComponent>("Kitchen Fire");
	KitchenFire->SetupAttachment(Mesh);
}

bool AKitchenFurniture::GetCanInteractive() const
{
	return !IsValid(GetInteractingPlayer());
}

FString AKitchenFurniture::GetInteractiveDisplayText() const
{
	return TEXT("요리하기");
}

void AKitchenFurniture::BeginPlay()
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

	KitchenFire->Deactivate();
}

void AKitchenFurniture::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		SetInteractingPlayer(Player);
		Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
		Player->bUseControllerRotationYaw = false;
		Player->SetActorTransform(
			KitchenStandPosition->GetComponentTransform());

		if (!Player->HasAuthority())
		{
			Player->Server_SetActorTransformReplicated(
				KitchenStandPosition->GetComponentTransform());
		}

		Player->GetInteractionComponent()->SetIsInteractive(false);

		FGameplayTagContainer ActivateTag;
		ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_StartCooking);
		Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			ActivateTag);
	}
}

void AKitchenFurniture::OnInteractiveResponse(AActor* Causer)
{
	Super::OnInteractiveResponse(Causer);
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);

	Server_SetInteractingPlayer(Player);
}

void AKitchenFurniture::EquipCookingToolToAct(
	const FEquipCookingToolToActParams& Params)
{
	switch (Params.CookingTool)
	{
	case ECookingTool::FryingPan:
		{
			FryPanMesh->SetHiddenInGame(false);
			FryPanMesh->AttachToComponent(Params.AttachPoint,
			                              FAttachmentTransformRules::SnapToTargetIncludingScale,
			                              TEXT("frying_pan_socket"));
			KitchenFire->Activate();
			break;
		}
	case ECookingTool::Wok:
		{
			ScoopMesh->SetHiddenInGame(false);
			ScoopMesh->AttachToComponent(Params.AttachPoint,
			                             FAttachmentTransformRules::SnapToTargetIncludingScale,
			                             TEXT("scoop_socket"));
			ScoopMesh->SetRelativeRotation(FRotator::ZeroRotator);

			WokMesh->SetHiddenInGame(false);
			WokMesh->AttachToComponent(Params.AttachPoint,
			                           FAttachmentTransformRules::SnapToTargetIncludingScale,
			                           TEXT("wok_socket"));
			WokMesh->SetRelativeRotation(FRotator::ZeroRotator);
			KitchenFire->Activate();

			break;
		}
	case ECookingTool::BigPot:
		{
			ScoopMesh->SetHiddenInGame(false);
			ScoopMesh->AttachToComponent(Params.AttachPoint,
			                             FAttachmentTransformRules::SnapToTargetIncludingScale,
			                             TEXT("scoop_socket"));
			ScoopMesh->SetRelativeRotation(FRotator::ZeroRotator);

			BigPotMesh->SetHiddenInGame(false);
			KitchenFire->Activate();

			break;
		}
	default:
		{
			break;
		}
	}
}

void AKitchenFurniture::UnEquipCookingToolToAct()
{
	FryPanMesh->SetHiddenInGame(true);
	FryPanMesh->AttachToComponent(GetRootComponent(),
	                              FAttachmentTransformRules::SnapToTargetIncludingScale);
	FryPanMesh->SetRelativeLocation(FVector::ZeroVector);

	ScoopMesh->SetHiddenInGame(true);
	ScoopMesh->AttachToComponent(GetRootComponent(),
	                             FAttachmentTransformRules::SnapToTargetIncludingScale);
	ScoopMesh->SetRelativeLocation(FVector::ZeroVector);

	WokMesh->SetHiddenInGame(true);
	WokMesh->AttachToComponent(GetRootComponent(),
	                           FAttachmentTransformRules::SnapToTargetIncludingScale);
	WokMesh->SetRelativeLocation(FVector::ZeroVector);

	BigPotMesh->SetHiddenInGame(true);
	KitchenFire->Deactivate();
}

void AKitchenFurniture::UnlockPlayer()
{
	Client_UnlockPlayer();

	if (HasAuthority())
	{
		SetInteractingPlayer(nullptr);
	}
}

void AKitchenFurniture::Client_UnlockPlayer_Implementation()
{
	if (GetInteractingPlayer() == nullptr)
	{
		return;
	}
	
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_EndCooking);
	GetInteractingPlayer()->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
		ActivateTag);
}

void AKitchenFurniture::Server_SetInteractingPlayer_Implementation(
	AMainPlayerCharacter* Player)
{
	SetInteractingPlayer(Player);
}
