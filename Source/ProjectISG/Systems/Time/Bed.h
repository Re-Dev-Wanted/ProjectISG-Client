// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "Bed.generated.h"

UCLASS()
class PROJECTISG_API ABed : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ABed();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual bool GetCanInteractive() const override;

private:
	void MovePlayerToBed();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MovePlayerToBed();

	UFUNCTION()
	void ActivateWakeUp();

	UFUNCTION()
	void OpenDiary();

	UFUNCTION()
	void ActivateSleepAbility();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_InteractiveValue(bool bEnable);

public:
	SETTER(class AMainPlayerCharacter*, MainPlayer);

	void SetCollisionEnabled(bool bEnable) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class USceneComponent* WakeUpPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class ATimeManager* TimeManager;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class AMainPlayerCharacter* MainPlayer;

	UPROPERTY(Replicated, EditAnywhere, Category = Setting,
		meta = (AllowPrivateAccess = true))
	bool CanInteractive = true;
};
