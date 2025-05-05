#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "DynamicEquipment.generated.h"

UCLASS()
class PROJECTISG_API ADynamicEquipment : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ADynamicEquipment();

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* AnchorComp;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* MeshComp;

};
