// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlayerHandSlotComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerHandSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerHandSlotComponent();

	GETTER(TObjectPtr<class ABaseActor>, HeldItem)

	UFUNCTION()
	void OnChange(uint16 ItemId);

	FString GetItemUsingType();
	
	bool IsHousingHandItem();

protected:
	virtual void InitializeComponent() override;

	TObjectPtr<class ABaseActor> HeldItem = nullptr;

	UPROPERTY()
	uint16 ItemId = 0;
};
