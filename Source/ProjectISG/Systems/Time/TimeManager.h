// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeManager.generated.h"

UCLASS()
class PROJECTISG_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeManager();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	void UpdateCycleTime(float DeltaTime);

	void UpdateCycleDate();

	void RotateSun();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_RotateSun(FRotator newRot);

public:

private:
#pragma region Sun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Sun")
	class UDirectionalLightComponent* Sun = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Sun")
	int32 SunriseTime = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Sun")
	int32 SunsetTime = 20;
#pragma endregion 
	
#pragma region Time
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	float Seconds = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Minutes = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Hours = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Days = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Months = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Year = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	TArray<int32> DaysInMonths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 SpeedOfTime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Time")
	bool TimeStop = false;
#pragma endregion
};
