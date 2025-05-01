// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "TimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSleep);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddSleepTimeToCrop);

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

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void UpdateCycleTime(float DeltaTime);

	void UpdateCycleDate();

	void RotateSun();

	UFUNCTION(BlueprintCallable)
	void StopTime(bool value);

	// 나중에 sleep class를 따로 만들어서 옮길 예정
#pragma region SleepFunc
	void Sleep();

	void ForceSleep();

	void SleepCinematic(float DeltaTime);

	void AssignBedEachPlayer();

	void ChangeAllPlayerSleepValue(bool value);

	bool CheckAllPlayerIsLieOnBed();
#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	void ChangeTimeToForceSleepTime();
	UFUNCTION(BlueprintCallable)
	void ChangeTimeToCanSleepTime();

	GETTER(int32, Day);
	GETTER(int32, Hour);
	GETTER(int32, Minute);
	GETTER(float, Second);
	GETTER(int32, TimeStoppedTime);

	FString GetDateText() const;
	uint32 GetTotalPlayingDay() const;

private:
#pragma region Sky
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Sky")
	class USceneComponent* Root = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Sky")
	FRotator SunRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Sky")
	int32 SunriseTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Sky")
	int32 SunsetTime = 20;
#pragma endregion

#pragma region Time
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	float Second = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Minute = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Hour = 0;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Day = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Month = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 Year = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	TArray<int32> DaysInMonths;

	// 5000이면 1초에 1시간 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 SpeedOfTime = 1;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	bool TimeStop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 WakeUpTime = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 CanSleepTime = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	int32 TimeStoppedTime = 0;
#pragma endregion

#pragma region Sleep
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	bool bSleepCinematicStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	float CinematicElapsedTime = 0.f;

	// 임시 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	float CinematicEndTime = 3.f;
#pragma endregion

public:
	UPROPERTY()
	FSleep SleepDelegate;

	UPROPERTY()
	FAddSleepTimeToCrop AddSleepTimeToCrop;
};
