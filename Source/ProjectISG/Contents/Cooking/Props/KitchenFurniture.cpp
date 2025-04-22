#include "KitchenFurniture.h"

AKitchenFurniture::AKitchenFurniture()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	KitchenStandPosition = CreateDefaultSubobject<USceneComponent>(
		"Kitchen Stand Position");
	KitchenStandPosition->SetupAttachment(Mesh);

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
}
