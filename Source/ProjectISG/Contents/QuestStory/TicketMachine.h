// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "TicketMachine.generated.h"

UCLASS()
class PROJECTISG_API ATicketMachine : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ATicketMachine();

protected:
	virtual void BeginPlay() override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetInteractiveDisplayText() const override;

private:
	void SetQuestInteractiveActorOverlayMaterial(bool value);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	FString NextQuest;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	bool bIsBoughtTicket = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UMaterial* QuestInteractMaterial;
};
