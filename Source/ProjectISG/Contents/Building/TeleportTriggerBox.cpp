#include "TeleportTriggerBox.h"

#include "TeleportPoint.h"
#include "Components/ArrowComponent.h"
#include "Components/ShapeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"


ATeleportTriggerBox::ATeleportTriggerBox()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATeleportTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, 
	&ATeleportTriggerBox::OnBeginOverlap);
	
}

auto ATeleportTriggerBox::Teleport(AMainPlayerCharacter* Player)
{
	return [this, Player]()
	{
		TArray<AActor*> Actors;

		UGameplayStatics::GetAllActorsWithTag(GetWorld(),
		                                      FName(*PointTag), Actors);

		for (auto It = Actors.CreateConstIterator(); It; ++It)
		{
			if (ATeleportPoint* Point = Cast<ATeleportPoint>(*It))
			{
				Player->SetActorLocation(Point->GetActorLocation());

				FRotator Rotation = Point->GetPoint()
				                         ->GetForwardVector()
				                         .Rotation();

				Player->SetActorRotation(Rotation);

				if (Player->Controller)
				{
					Player->Controller->SetControlRotation(Rotation);
				}
				break;
			}
		}
	};
}

void ATeleportTriggerBox::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(OtherActor))
	{
		FLatentActionInfo LatentActionInfo;
		
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), 
		StreamingLevel, true, false, LatentActionInfo);

		FTimerHandle TimerHandle;

		GetWorld()
			->GetTimerManager()
			.SetTimer
			(
				TimerHandle,
				Teleport(Player),
				0.25f,
				false
			);
	}
}
