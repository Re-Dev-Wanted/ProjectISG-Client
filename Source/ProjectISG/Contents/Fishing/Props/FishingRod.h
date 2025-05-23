// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Fishing/FishData.h"
#include "ProjectISG/Systems/Water/Actors/DynamicEquipment.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "FishingRod.generated.h"

class ABobber;
class UCableComponent;

UCLASS()
class PROJECTISG_API AFishingRod : public ADynamicEquipment
{
	GENERATED_BODY()

public:
	AFishingRod();

	GETTER(bool, IsBiteFish)
	GETTER(FFishData, FishData)

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
	void OnStartFishing();

	UFUNCTION()
	void OnEventBite();

	UFUNCTION()
	void OnEventRealBite();
	
	UFUNCTION()
	void OnEventFinish(bool bLoop);

	TArray<FTimerHandle> TimerHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABobber> BobberFactory;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SocketComp;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PocketSocketComp; 

	UPROPERTY(VisibleAnywhere)
	USceneComponent* CastingStartPoint;
	
	UPROPERTY(VisibleAnywhere)
	UCableComponent* Thread;
	
	UPROPERTY()
	TObjectPtr<ABobber> Bobber;

	bool IsBiteFish = false;
	
	bool IsInWater = false;

	// 지금 낚시대에 물고 있는 물고기 데이터
	FFishData FishData;

	void BiteLogging();
	
	void FinishLogging(bool bSuccess);
public:
	void StartCasting(AActor* Causer, FVector Destination);

	void ReelInLine();

	void OnEndReelInLine(AActor* Causer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FishingSettings)
	float CastingDistance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMax = 1.f), Category = FishingSettings)
	float BitingCheckDelayTime = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin = 0.5f, ClampMax = 2.f), Category = FishingSettings)
	float BitingTime = 1.f;
	
};
