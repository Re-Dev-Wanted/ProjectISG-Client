// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ProjectISG/GAS/Common/Object/BaseInteractiveActor.h>

#include "CoreMinimal.h"
#include "TradingNPC.generated.h"

UCLASS()
class PROJECTISG_API ATradingNPC : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ATradingNPC();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetDisplayText() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	class USceneComponent* InteractionPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Settings,
		meta = (AllowPrivateAccess = true))
	bool CanInteractive = false;
};
