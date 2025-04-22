#include "KitchenFurniture.h"

#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
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

	Causer->SetActorTransform(KitchenStandPosition->GetComponentTransform());

	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_StartCooking);

	if (const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			ActivateTag);
	}
}
