#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "KitchenFurniture.generated.h"

class UCameraComponent;
class UCookingComponent;

UCLASS()
class PROJECTISG_API AKitchenFurniture : public ABaseActor
{
	GENERATED_BODY()

public:
	AKitchenFurniture();

	virtual void OnInteractive(AActor* Causer) override;

	GETTER(TObjectPtr<UCameraComponent>, KitchenCameraComponent)

	virtual bool GetCanInteractive() const override;

	virtual FString GetDisplayText() const override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> KitchenStandPosition;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> KitchenCameraComponent;
};
