// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridIndicatorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UGridIndicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGridIndicatorComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnUpdateSelectedItem(bool bIsEquippable, TSubclassOf<AActor> 
	ActorClass, FName SocketName = NAME_None);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IndicatorProperties)
	float InterpSpeed = 5.f;

	UPROPERTY()
	TObjectPtr<class APlayerController> PlayerController = nullptr;

	UPROPERTY()
	class APlacement* IndicateActor = nullptr;

	UFUNCTION()
	virtual void OnActivate(const TSubclassOf<AActor>& Factory);

	UFUNCTION()
	virtual void OnDeactivate();

	UFUNCTION(BlueprintCallable)
	virtual void Execute();

	UFUNCTION(Server, Reliable)
	void Server_Execute(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass);

	UFUNCTION()
	virtual void ExecuteInternal(FVector Pivot, FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass);
};
