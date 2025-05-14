// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "TeleportTriggerBox.generated.h"

UCLASS()
class PROJECTISG_API ATeleportTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ATeleportTriggerBox();

protected:
	virtual void BeginPlay() override;
	
	auto Teleport(AMainPlayerCharacter* Player);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> StreamingLevel = nullptr;

	UPROPERTY(EditAnywhere)
	FString PointTag;
};
