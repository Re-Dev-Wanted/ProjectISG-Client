#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "DynamicEquipment.generated.h"

UCLASS()
class PROJECTISG_API ADynamicEquipment : public ABaseActor
{
	GENERATED_BODY()

public:
	ADynamicEquipment();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* MeshComp;

};
