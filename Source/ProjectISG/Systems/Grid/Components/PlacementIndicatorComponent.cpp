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

	bool bIsBlock = false;

	if (GhostPlacement)
	{
		FVector SnappedLocation = GridManager->GetLocationInPointerDirectionPlacement(
			PlayerController, GhostPlacement->GetMeshSize());


		GhostPlacement->SetActorLocation(FMath::VInterpTo(GhostPlacement->GetActorLocation(), SnappedLocation,
		                                                  0.1f,
		                                                  InterpSpeed));
		GhostPlacement->SetActorRotation(FMath::RInterpTo(GhostPlacement->GetActorRotation(),
		                                                  FRotator(0, GetDegrees(RotateDirection), 0), 0.1f,
		                                                  InterpSpeed));
		FIntVector GridCoord;
		APlacement* PlacedActor;

		bIsBlock = GridManager->TryGetPlacement(SnappedLocation, GridCoord, PlacedActor);

		GhostPlacement->SetColor(true, bIsBlock);
	}
}

void UPlacementIndicatorComponent::OnActivate(const TSubclassOf<AActor>& Factory)
{
	SetActive(true);

	if (GhostPlacement)
	{
		GhostPlacement->Destroy();
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(Factory);

	GhostPlacement = Cast<APlacement>(Actor);
	GhostPlacement->Setup(GridManager->SnapSize);
}

void UPlacementIndicatorComponent::OnDeactivate()
{
	if (GhostPlacement)
	{
		GhostPlacement->Destroy();
	}

	SetActive(false);
}

void UPlacementIndicatorComponent::Build()
{
	if (GridManager)
	{
		const TSubclassOf<APlacement> PlacementFactory = GhostPlacement->GetClass();
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		GridManager->BuildPlacementAtGhost(PlacementFactory, GhostPlacement);
	}
}

void UPlacementIndicatorComponent::Remove()
{
	if (GridManager && GhostPlacement)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *GhostPlacement->GetActorLocation().ToCompactString());
		FIntVector GridCoord;
		APlacement* PlacedActor;

		if (GridManager->TryGetPlacement(GhostPlacement, GridCoord, PlacedActor))
		{
			// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s"), *GridCoord.ToString()));
			GridManager->RemovePlacement(GridCoord);
		}
	}
}

void UPlacementIndicatorComponent::Rotate(bool bClockwise)
{
	if (bClockwise)
	{
		RotateDirection = RotateDirection << static_cast<uint8>(1);
		return;
	}
	RotateDirection = RotateDirection >> static_cast<uint8>(1);
}
