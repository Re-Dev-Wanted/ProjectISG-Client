// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "BaseInteractiveActor.generated.h"

UCLASS()
class PROJECTISG_API ABaseInteractiveActor : public ABaseActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	GETTER_SETTER(class AMainPlayerCharacter*, InteractingPlayer)

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_SetInteractingPlayer(class AMainPlayerCharacter* Player);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_SetInteractingPlayer, EditAnywhere, meta = (AllowPrivateAccess = true))
	class AMainPlayerCharacter* InteractingPlayer = nullptr;
};
