// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlayerHandSlotComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerHandSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerHandSlotComponent();

	GETTER(class ABaseActor*, EmptyItem)

	UFUNCTION()
	void OnChange(bool bIsEquippable, TSubclassOf<AActor> ActorClass = nullptr, 
	FName SocketName = NAME_None);
	
	void OnAttackAction(AActor* Causer);
	
	void OnInteractAction(AActor* Causer);
	
	void OnTouchAction(AActor* Causer);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	UPROPERTY()
	class ABaseActor* EmptyItem;

	TObjectPtr<class ABaseActor> HeldItem;
};
