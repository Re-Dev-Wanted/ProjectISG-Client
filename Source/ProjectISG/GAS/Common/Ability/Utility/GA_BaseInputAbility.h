#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "GA_BaseInputAbility.generated.h"

class UInputAction;

UENUM()
enum class EAbilityInputId : uint8
{
	Undefined
};

UCLASS()
class PROJECTISG_API UGA_BaseInputAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BaseInputAbility(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere
		, Category = "Custom Gameplay Ability")
	bool ActivateAbilityOnGranted = false;

	GETTER(EAbilityInputId, InputId)

protected:
	uint32 TriggeredEventHandle = -1;

	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo
	                                , const FGameplayAbilitySpec& Spec);

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo
	                         , const FGameplayAbilitySpec& Spec) override;

	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle
	                        , const FGameplayAbilityActorInfo* ActorInfo
	                        , const FGameplayAbilityActivationInfo
	                        ActivationInfo
	                        , bool bReplicateEndAbility
	                        , bool bWasCancelled) override;

	

	UPROPERTY(EditDefaultsOnly, Category = "Option",
		meta = (AllowPrivateAccess = true))
	bool IsPassive;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Option|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "!IsPassive"))
	TObjectPtr<UInputAction> ActivationInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Option|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "!IsPassive"))
	EAbilityInputId InputId = EAbilityInputId::Undefined;

	void OnAbilityInputPressed(const FGameplayAbilityActorInfo* ActorInfo);

	void OnAbilityInputReleased(const FGameplayAbilityActorInfo* ActorInfo);
};
