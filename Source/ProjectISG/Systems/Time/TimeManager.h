// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "TimeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddSleepTimeToCrop);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContentRestrictionTimeReached);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContentRestrictionCancelTimeReached);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnForceSleepTimeAlmostReached);


UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Morning,
	Afternoon,
	Evening,
	Night,
};

UCLASS()
class PROJECTISG_API ATimeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeManager();

	UFUNCTION(BlueprintCallable)
	void StopTime(bool value);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ResetAllPlayerWidget();

	UFUNCTION()
	void PushSleepAlertWidget();

private:
	void UpdateCycleTime(float DeltaTime);

	void UpdateCycleDate();

	void UpdateTimeOfDay(ETimeOfDay TOD);

	bool CheckTimeUI();

	void RotateSun();

	UFUNCTION()
	void OnRep_CurrentTimeOfDay();

	void ChangeTOD();


#pragma region Log
	void LoggingToMorning();

	void LoggingToAfternoon();

	void LoggingToEvening();

	void LoggingToNight();
#pragma endregion

public:
	void ChangeDayBySleep();

	GETTER(int32, Day);
	GETTER(int32, Hour);
	GETTER(int32, Minute);
	GETTER(float, Second);
	GETTER(int32, TimeStoppedTime);
	GETTER(class USleepManager*, SleepManager);

	FString GetDateText() const;
	FString GetDateDisplayText() const;
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
	int32 SunriseTime = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Sky")
	int32 SunsetTime = 20;
#pragma endregion

#pragma region Time
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTimeOfDay, EditAnywhere,
		BlueprintReadOnly,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	ETimeOfDay CurrentTimeOfDay = ETimeOfDay::Morning;

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
	int32 TimeStoppedTime = 0;
#pragma endregion

#pragma region Sleep
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess),
		Category = Sleep)
	class USleepManager* SleepManager = nullptr;
#pragma endregion

#pragma region UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	class UUIV_Time* TimeView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	class UUIC_Time* TimeController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = "Time")
	class UUIM_Time* TimeModel = nullptr;
#pragma endregion

public:
	UPROPERTY()
	FAddSleepTimeToCrop AddSleepTimeToCrop;

	UPROPERTY()
	FOnContentRestrictionTimeReached OnContentRestrictionTimeReached;

	UPROPERTY()
	FOnContentRestrictionCancelTimeReached
	OnContentRestrictionCancelTimeReached;

	UPROPERTY()
	FOnForceSleepTimeAlmostReached OnForceSleepTimeAlmostReached;

	UPROPERTY(EditAnywhere)
	bool bIsLobbyTimer = false;
};
