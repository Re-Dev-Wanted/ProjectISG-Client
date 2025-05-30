#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishActor.generated.h"

UCLASS()
class PROJECTISG_API AFishActor : public AActor
{
	GENERATED_BODY()

public:
	AFishActor();

	void SetMesh(TObjectPtr<UStaticMesh> Mesh);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
