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

	GETTER(class ABaseActor*, EmptyItem)

	UFUNCTION()
	void OnChange(TSubclassOf<AActor> ActorClass, FItemMetaInfo _ItemMetaInfo);
	
	void OnInteractAction(AActor* Causer) const;
	
	void OnTouchAction(AActor* Causer) const;

	bool IsHousingHandItem();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	UPROPERTY()
	class ABaseActor* EmptyItem = nullptr;

	TObjectPtr<class ABaseActor> HeldItem = nullptr;

	UPROPERTY()
	FItemMetaInfo ItemMetaInfo;
};
