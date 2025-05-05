// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "SleepManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSleep);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API USleepManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USleepManager();

protected:
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

private:
	void Sleep();

	void ForceSleep();

	void SleepCinematic(float DeltaTime);

	void AssignBedEachPlayer();

	void ChangeAllPlayerSleepValue(bool value);

	bool CheckAllPlayerIsLieOnBed();
	
public:
	GETTER(bool, bSleepCinematicStart);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = Setting)
	class ATimeManager* TimeManager;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = Sleep)
	int32 CanSleepTime = 9;

public:
	UPROPERTY()
	FSleep SleepDelegate;
};
