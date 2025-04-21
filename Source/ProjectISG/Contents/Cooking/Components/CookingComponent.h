#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CookingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UCookingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCookingComponent();

	void OnInteractive();

protected:
	virtual void BeginPlay() override;
};
