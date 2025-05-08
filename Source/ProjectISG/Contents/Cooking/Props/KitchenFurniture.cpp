#include "KitchenFurniture.h"

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

AKitchenFurniture::AKitchenFurniture()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	KitchenStandPosition = CreateDefaultSubobject<USceneComponent>(
		"Kitchen Stand Position");
	KitchenStandPosition->SetupAttachment(Mesh);

	KitchenCameraComponent = CreateDefaultSubobject<UCameraComponent>(
		"Kitchen Camera Component");
	KitchenCameraComponent->SetupAttachment(Mesh);


	FryPanMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Fry Pan Mesh");
	FryPanMesh->SetupAttachment(GetRootComponent());

	ScoopMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Scoop Mesh");
	ScoopMesh->SetupAttachment(GetRootComponent());

	WokMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Wok Mesh");
	WokMesh->SetupAttachment(GetRootComponent());
}

bool AKitchenFurniture::GetCanInteractive() const
{
	return true;
}

FString AKitchenFurniture::GetInteractiveDisplayText() const
{
	return TEXT("요리하기");
}

void AKitchenFurniture::BeginPlay()
{
	Super::BeginPlay();
}

void AKitchenFurniture::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
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

void AKitchenFurniture::EquipCookingToolToAct(
	const FEquipCookingToolToActParams& Params)
{
	switch (Params.CookingTool)
	{
	case ECookingTool::FryingPan:
		{
			FryPanMesh->SetVisibility(true);
			FryPanMesh->AttachToComponent(Params.AttachPoint,
			                              FAttachmentTransformRules::SnapToTargetIncludingScale,
			                              TEXT("hand_r"));
			break;
		}
	case ECookingTool::Wok:
		{
			ScoopMesh->SetVisibility(true);
			ScoopMesh->AttachToComponent(Params.AttachPoint,
			                             FAttachmentTransformRules::SnapToTargetIncludingScale,
			                             TEXT("hand_l"));

			WokMesh->SetVisibility(true);
			WokMesh->AttachToComponent(Params.AttachPoint,
			                           FAttachmentTransformRules::SnapToTargetIncludingScale,
			                           TEXT("hand_r"));
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
	FryPanMesh->AttachToComponent(GetRootComponent(),
	                              FAttachmentTransformRules::SnapToTargetIncludingScale);

	ScoopMesh->SetVisibility(false);
	ScoopMesh->AttachToComponent(GetRootComponent(),
	                             FAttachmentTransformRules::SnapToTargetIncludingScale);

	WokMesh->SetVisibility(false);
	WokMesh->AttachToComponent(GetRootComponent(),
	                           FAttachmentTransformRules::SnapToTargetIncludingScale);
}
