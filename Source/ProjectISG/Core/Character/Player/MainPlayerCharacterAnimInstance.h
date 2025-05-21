#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class AMainPlayerCharacter;

UCLASS()
class PROJECTISG_API UMainPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AMainPlayerCharacter> Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundSpeed_H;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundSpeed_V;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
