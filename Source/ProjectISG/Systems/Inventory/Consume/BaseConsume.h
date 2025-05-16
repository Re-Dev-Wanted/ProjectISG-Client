// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "BaseConsume.generated.h"

UCLASS()
class PROJECTISG_API ABaseConsume : public ABaseActor
{
	GENERATED_BODY()

public:
	ABaseConsume();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Setting)
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true), Category = Setting)
	class UStaticMeshComponent* Mesh;

};
