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

	FIntVector GridCoord;
	APlacement* PlacedActor;

	bool bIsBlock = GridManager->TryGetPlacement(SnappedLocation, GridCoord, PlacedActor);

	if (GhostPlacement)
	{
		GhostPlacement->SetActorLocation(SnappedLocation);
		GhostPlacement->SetActorEnableCollision(false);
		GhostPlacement->SetColor(true, bIsBlock);
	}
}

void UPlacementIndicatorComponent::Build()
{
	if (GridManager)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		GridManager->BuildPlacement(PlacementFactory, GhostPlacement->GetActorLocation(),
		                            GhostPlacement->GetActorRotation());
	}
}

void UPlacementIndicatorComponent::Remove()
{
	if (GridManager)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		FIntVector GridCoord;
		APlacement* PlacedActor;

		if (GridManager->TryGetPlacement(GhostPlacement->GetActorLocation(), GridCoord, PlacedActor))
		{
			// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *GridCoord.ToString()));
			GridManager->RemovePlacement(GridCoord);
		}
	}
}

void UPlacementIndicatorComponent::Rotate()
{
	GhostPlacement->AddActorLocalRotation(FRotator(0, 90, 0));
}
