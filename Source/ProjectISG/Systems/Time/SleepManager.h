#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "SleepManager.generated.h"

struct FDiaryLogParams;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSleep);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWakeUp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDiary);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FForceSleep);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API USleepManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USleepManager();

protected:
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

private:
	void Sleep();

	void ForceSleep();

	void SleepCinematic(float DeltaTime);

	UFUNCTION()
	void OnRep_SleepCinematicStart();

	UFUNCTION()
	void LoadHouseLevel();

	void AssignBedEachPlayer();

	void ChangeAllPlayerSleepValue(bool value);

	void ChangeAllPlayerLieInBedValue(bool value);

	bool CheckAllPlayerIsLieInBed();

	bool CheckAllPlayerWriteDiary();

	void ChangeAllPlayerWriteDiary(bool value);

	void LoggingToSleep();

	void LoggingToWakeUp();

public:
	GETTER(bool, bSleepCinematicStart)

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = Setting)
	class ATimeManager* TimeManager;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess), Category = Setting)
	TSoftObjectPtr<UWorld> HouseLevel = nullptr;

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
	float CinematicEndTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = Sleep)
	int32 CanSleepTime = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		meta = (AllowPrivateAccess = "true"), Category = Sleep)
	bool bUseForceSleep = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,
	meta = (AllowPrivateAccess = "true"), Category = Sleep)
	bool bUseSleepManager = true;

public:
	UPROPERTY()
	FForceSleep ForceSleepDelegate;
	
	UPROPERTY()
	FOpenDiary OpenDiaryDelegate;

	UPROPERTY()
	FWakeUp WakeUpDelegate;

};
