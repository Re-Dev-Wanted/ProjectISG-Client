#include "TeleportPoint.h"

#include "Components/ArrowComponent.h"

ATeleportPoint::ATeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Point = CreateDefaultSubobject<UArrowComponent>(TEXT("Root"));
	SetRootComponent(Point);
}

