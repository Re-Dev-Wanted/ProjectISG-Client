// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "GranmaHouseDiary.generated.h"

UCLASS()
class PROJECTISG_API AGranmaHouseDiary : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	AGranmaHouseDiary();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual bool GetCanInteractive() const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void LoadPlayLevelAndTeleportPlayer(class AMainPlayerCharacter* Player);

private:
	void SetQuestInteractiveActorOverlayMaterial(bool value);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UStaticMeshComponent* Book;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UStaticMeshComponent* Desk;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	class UMaterial* QuestInteractMaterial;
	
};
