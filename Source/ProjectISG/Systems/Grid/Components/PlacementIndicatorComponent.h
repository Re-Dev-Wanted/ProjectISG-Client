// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlacementIndicatorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlacementIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlacementIndicatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class AGridManager* GridManager;

	UPROPERTY()
	class APlacement* GhostPlacement;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlacement> PlacementFactory;

	UFUNCTION(BlueprintCallable)
	void Build();

	UFUNCTION(BlueprintCallable)
	void Remove();
};
