#include "KitchenFurniture.h"

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
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

	CanInteractive = true;
}

void AKitchenFurniture::BeginPlay()
{
	Super::BeginPlay();

	DisplayText = TEXT("요리하기");
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
