// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "BaseInteractiveActor.generated.h"

UCLASS()
class PROJECTISG_API ABaseInteractiveActor : public ABaseActor, public IInteractionInterface
{
	GENERATED_BODY()
	
};
