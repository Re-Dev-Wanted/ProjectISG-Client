#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "KitchenFurniture.generated.h"

class UCookingComponent;

UCLASS()
class PROJECTISG_API AKitchenFurniture : public ABaseActor,
                                         public IInteractionInterface
{
	GENERATED_BODY()

public:
	AKitchenFurniture();

	virtual void OnInteractive(AActor* Causer) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> KitchenStandPosition;
};
