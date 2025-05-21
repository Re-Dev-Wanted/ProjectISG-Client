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
	bool bPaidTicket = false;

	FString DisplayText;
};
