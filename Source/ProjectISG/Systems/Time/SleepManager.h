// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "SleepManager.generated.h"

struct FDiaryLogParams;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSleep);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWakeUp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDiary);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API USleepManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USleepManager();

protected:
	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

private:
	void Sleep();

	void ForceSleep();

	void SleepCinematic(float DeltaTime);

	UFUNCTION()
	void OnRep_SleepCinematicStart();

	void AssignBedEachPlayer();

	void ChangeAllPlayerSleepValue(bool value);

	void ChangeAllPlayerLieInBedValue(bool value);

	bool CheckAllPlayerIsLieInBed();

	void LoggingToSleep();

	void LoggingToWakeUp();

public:
	GETTER(bool, bSleepCinematicStart);
	GETTER(float, CinematicEndTime);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = Setting)
	class ATimeManager* TimeManager;

	UPROPERTY(ReplicatedUsing = OnRep_SleepCinematicStart, EditAnywhere,
		BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	bool bSleepCinematicStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	float CinematicElapsedTime = 0.f;

	// 임시 변수, 시네마틱이 추가 되면 해당 시네마틱 시간을 가져와야함
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = true), Category = Sleep)
	float CinematicEndTime = 999.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = Sleep)
	int32 CanSleepTime = 9;

public:
	UPROPERTY()
	FSleep SleepDelegate;

	UPROPERTY()
	FWakeUp WakeUpDelegate;

	UPROPERTY()
	FOpenDiary OpenDiaryDelegate;
};
