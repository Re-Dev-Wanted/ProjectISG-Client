#include "PlacementIndicatorComponent.h"

#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"


UPlacementIndicatorComponent::UPlacementIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlacementIndicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	GhostPlacement = GetWorld()->SpawnActor<APlacement>(PlacementFactory);
	GhostPlacement->Setup(GridManager->SnapSize);
}


// Called every frame
void UPlacementIndicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GridManager || !PlayerController)
	{
		return;
	}

	FVector SnappedLocation = GridManager->GetLocationInPointerDirection(PlayerController);

	if (GhostPlacement)
	{
		GhostPlacement->SetActorLocation(SnappedLocation);
		GhostPlacement->SetActorEnableCollision(false);
	}
}

void UPlacementIndicatorComponent::Build()
{
	if (GridManager)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		GridManager->BuildPlacement(PlacementFactory, GhostPlacement->GetActorLocation());
	}
}

void UPlacementIndicatorComponent::Remove()
{
	if (GridManager)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		FIntVector GridCoord;
		APlacement* PlacedActor;
		if (GridManager->TryGetPlacementAt(GetOwner(), GridCoord, PlacedActor))
		{
			GridManager->RemovePlacement(GridCoord);
		}
	}
}
