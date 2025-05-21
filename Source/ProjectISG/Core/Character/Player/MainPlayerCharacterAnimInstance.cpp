#include "MainPlayerCharacterAnimInstance.h"

#include "MainPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMainPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<AMainPlayerCharacter>(GetOwningActor());
	if (!Player)
	{
		return;
	}
	MovementComponent = Player->GetCharacterMovement();
}

void UMainPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Player || !MovementComponent)
	{
		return;
	}

	GroundSpeed_H = FVector::DotProduct(Player->GetVelocity(), Player->GetActorRightVector());
	GroundSpeed_V = FVector::DotProduct(Player->GetVelocity(), Player->GetActorForwardVector());

	if (GroundSpeed_H != 0 || GroundSpeed_V != 0)
	{
		bShouldMove = MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;
	}

	bIsFalling = MovementComponent->IsFalling();
}
