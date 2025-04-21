#include "CookingComponent.h"


UCookingComponent::UCookingComponent()
{
}


void UCookingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCookingComponent::OnInteractive()
{
	UE_LOG(LogTemp, Display, TEXT("요리 스타트"));
}
