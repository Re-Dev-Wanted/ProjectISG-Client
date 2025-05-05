#include "Bobber.h"

#include "Components/BoxComponent.h"

ABobber::ABobber()
{
	LineAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LineAttachPoint"));
	LineAttachPoint->SetupAttachment(RootComponent);

	Root->SetSimulatePhysics(false);
}
