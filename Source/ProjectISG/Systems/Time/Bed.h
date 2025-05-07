// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
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

	virtual FString GetDisplayText() const override;

	virtual bool GetCanInteractive() const override;

private:
	UFUNCTION()
	void MovePlayerToBed();

	UFUNCTION(Server, Reliable)
	void Server_MovePlayerToBed();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MovePlayerToBed();

	UFUNCTION()
	void MovePlayerToWakeUpPos();

public:
	SETTER(class AMainPlayerCharacter*, MainPlayer);

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
	class ATimeManager* TimeManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting,
		meta = (AllowPrivateAccess = true))
	class AMainPlayerCharacter* MainPlayer;

	UPROPERTY(Replicated, EditAnywhere, Category = Setting,
		meta = (AllowPrivateAccess = true))
	bool CanInteractive = true;
};
