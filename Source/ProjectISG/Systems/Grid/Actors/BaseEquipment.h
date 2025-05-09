#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "BaseEquipment.generated.h"

UCLASS()
class PROJECTISG_API ABaseEquipment : public ABaseActor
{
	GENERATED_BODY()

public:
	ABaseEquipment();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComp;
};
