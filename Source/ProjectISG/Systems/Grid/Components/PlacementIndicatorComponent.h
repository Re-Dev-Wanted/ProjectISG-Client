// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Grid/PlacementData.h"
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

	UFUNCTION()
	void OnUpdateSelectedItem(bool bIsEquippable, TSubclassOf<AActor> 
	ActorClass, FName SocketName = NAME_None);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IndicatorProperties)
	float InterpSpeed = 5.f;

	UPROPERTY()
	TEnumAsByte<ERotateDirection> RotateDirection = North;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY()
	class AGridManager* GridManager;

	UPROPERTY()
	class APlacement* GhostPlacement;

	UFUNCTION()
	void OnActivate(const TSubclassOf<AActor>& Factory);

	UFUNCTION()
	void OnDeactivate();

	UFUNCTION(BlueprintCallable)
	void Build();

	UFUNCTION(Server, Reliable)
	void Server_Build(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass);

	UFUNCTION(BlueprintCallable)
	void Remove();

	UFUNCTION(BlueprintCallable)
	void Rotate(bool bClockwise);
};
