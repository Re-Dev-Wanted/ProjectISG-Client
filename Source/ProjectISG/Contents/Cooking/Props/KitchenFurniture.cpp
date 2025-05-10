#include "KitchenFurniture.h"

#include "Camera/CameraComponent.h"
#include "Component/KitchenFurnitureCinematicComponent.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"

AKitchenFurniture::AKitchenFurniture()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	KitchenStandPosition = CreateDefaultSubobject<USceneComponent>(
		"Kitchen Stand Position");
	KitchenStandPosition->SetupAttachment(Mesh);

	KitchenCameraComponent = CreateDefaultSubobject<UCameraComponent>(
		"Kitchen Camera Component");
	KitchenCameraComponent->SetRelativeLocation({140.0, 90.0, 90.0});
	KitchenCameraComponent->SetRelativeRotation({0.0, 180.0, 0.0});
	KitchenCameraComponent->SetupAttachment(Mesh);

	FryPanMesh = CreateDefaultSubobject<UStaticMeshComponent>("Fry Pan Mesh");
	FryPanMesh->SetupAttachment(Mesh);

	ScoopMesh = CreateDefaultSubobject<UStaticMeshComponent>("Scoop Mesh");
	ScoopMesh->SetupAttachment(Mesh);

	WokMesh = CreateDefaultSubobject<UStaticMeshComponent>("Wok Mesh");
	WokMesh->SetupAttachment(Mesh);

	KitchenFurnitureCinematicComponent = CreateDefaultSubobject<
		UKitchenFurnitureCinematicComponent>(
		"Kitchen Furniture Cinematic Component");

	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);
}

bool AKitchenFurniture::GetCanInteractive() const
{
	return GetOwner() == nullptr;
}

FString AKitchenFurniture::GetInteractiveDisplayText() const
{
	return TEXT("요리하기");
}

void AKitchenFurniture::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		// RPC 함수를 위한 Owner 세팅
		SetOwner(Player);

		SetUsingOwner(Player);

		Player->bUseControllerRotationYaw = false;
		Player->SetActorTransform(
			KitchenStandPosition->GetComponentTransform());

		if (!Player->HasAuthority())
		{
			Player->Server_SetActorTransformReplicated(
				KitchenStandPosition->GetComponentTransform());
		}

		Player->GetInteractionComponent()->SetIsInteractive(false);
		Player->GetInteractionComponent()->SetSelectedInteractiveActor(this);

		PlayCookingCinematic(
			Player, EKitchenFurnitureCinematicStatus::StartCooking);
	}
}

void AKitchenFurniture::PlayCookingCinematic(AMainPlayerCharacter* Target
											, const
											EKitchenFurnitureCinematicStatus
											Status)
{
	if (Target->IsLocallyControlled())
	{
		PlayCookingCinematic_Internal(Target, Status, true);
	}

	if (HasAuthority())
	{
		Multicast_PlayCookingCinematic(Target, Status);
	}
	else
	{
		// 알수 없는 이유로 클라이언트에서 RPC 통신이 안됨
		Server_PlayCookingCinematic(Target, Status);
	}
}

void AKitchenFurniture::PlayCookingCinematic_Internal(
	AMainPlayerCharacter* Target, const EKitchenFurnitureCinematicStatus Status
	, const bool IsCaster)
{
	switch (Status)
	{
	case EKitchenFurnitureCinematicStatus::StartCooking:
		{
			KitchenFurnitureCinematicComponent->StartCookingMode(
				Target, IsCaster);
			break;
		}
	case EKitchenFurnitureCinematicStatus::EndCooking:
		{
			KitchenFurnitureCinematicComponent->
				EndCookingMode(Target, IsCaster);
			break;
		}
	default:
		{
			break;
		}
	}
}

void AKitchenFurniture::Server_PlayCookingCinematic_Implementation(
	AMainPlayerCharacter* Target, const EKitchenFurnitureCinematicStatus Status)
{
	Multicast_PlayCookingCinematic(Target, Status);
}

void AKitchenFurniture::Multicast_PlayCookingCinematic_Implementation(
	AMainPlayerCharacter* Target, const EKitchenFurnitureCinematicStatus Status)
{
	PlayCookingCinematic_Internal(Target, Status, false);
}

void AKitchenFurniture::EquipCookingToolToAct(
	const FEquipCookingToolToActParams& Params)
{
	switch (Params.CookingTool)
	{
	case ECookingTool::FryingPan:
		{
			FryPanMesh->SetVisibility(true);
			FryPanMesh->AttachToComponent(Params.AttachPoint
										, FAttachmentTransformRules::SnapToTargetIncludingScale
										, TEXT("hand_r"));
			break;
		}
	case ECookingTool::Wok:
		{
			ScoopMesh->SetVisibility(true);
			ScoopMesh->AttachToComponent(Params.AttachPoint
										, FAttachmentTransformRules::SnapToTargetIncludingScale
										, TEXT("scoop_socket"));
			ScoopMesh->SetRelativeRotation(FRotator::ZeroRotator);

			WokMesh->SetVisibility(true);
			WokMesh->AttachToComponent(Params.AttachPoint
										, FAttachmentTransformRules::SnapToTargetIncludingScale
										, TEXT("wok_socket"));
			WokMesh->SetRelativeRotation(FRotator::ZeroRotator);

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
	FryPanMesh->SetVisibility(false);
	FryPanMesh->AttachToComponent(
		Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	FryPanMesh->SetRelativeLocation(FVector::ZeroVector);

	ScoopMesh->SetVisibility(false);
	ScoopMesh->AttachToComponent(
		Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ScoopMesh->SetRelativeLocation(FVector::ZeroVector);

	WokMesh->SetVisibility(false);
	WokMesh->AttachToComponent(
		Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
	WokMesh->SetRelativeLocation(FVector::ZeroVector);
}
