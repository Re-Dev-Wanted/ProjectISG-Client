// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "PlayerHandSlotComponent.generated.h"

class AMainPlayerCharacter;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerHandSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerHandSlotComponent();

	GETTER(TObjectPtr<class ABaseActor>, HeldItem)
	GETTER(uint16, ItemId)

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnChange(uint16 ItemId);

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);

	FString GetItemUsingType();
	
	bool IsHousingHandItem();

	void SetIsUseInputAction(const bool NewIsUseInputAction);

	UFUNCTION(Server, Reliable)
	void Server_ChangeItemId(uint16 ChangeItemId);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ChangeItemId(uint16 ChangeItemId);

protected:
	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnAction();

	UPROPERTY()
	TObjectPtr<AMainPlayerCharacter> Player;
	
	UPROPERTY(Replicated, EditAnywhere)
	uint16 ItemId = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TouchAction;

	bool IsUseInputAction = false;

	UPROPERTY(Replicated)
	TObjectPtr<class ABaseActor> HeldItem = nullptr;
};
