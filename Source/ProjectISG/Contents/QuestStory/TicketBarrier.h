// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "TicketBarrier.generated.h"

UCLASS()
class PROJECTISG_API ATicketBarrier : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ATicketBarrier();

	UFUNCTION(BlueprintCallable)
	void SetQuestInteractiveActorOverlayMaterial(bool value);

	UFUNCTION(BlueprintImplementableEvent)
	void SetVisibilityBillboard(bool value);
	
protected:
	virtual void BeginPlay() override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetInteractiveDisplayText() const override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
	Category = Setting)
	class UStaticMeshComponent* Mesh0;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
	Category = Setting)
	class UStaticMeshComponent* Mesh1;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
	Category = Setting)
	class UStaticMeshComponent* Mesh2;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
	Category = Setting)
	class UStaticMeshComponent* Mesh3;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UMaterial* QuestInteractMaterial;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	bool bPaidTicket = false;

	FString DisplayText;
	
};
