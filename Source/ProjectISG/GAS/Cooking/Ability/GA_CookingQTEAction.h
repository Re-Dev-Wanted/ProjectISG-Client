#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_CookingQTEAction.generated.h"


class UAT_LogWithScreenShot;
class UAT_PlayCinematic;
class ULevelSequence;
class ALevelSequenceActor;

UCLASS()
class PROJECTISG_API UGA_CookingQTEAction : public UGA_BaseInputAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle
	                             , const FGameplayAbilityActorInfo* ActorInfo
	                             , const FGameplayAbilityActivationInfo
	                             ActivationInfo
	                             , const FGameplayEventData*
	                             TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle
	                        , const FGameplayAbilityActorInfo* ActorInfo
	                        , const FGameplayAbilityActivationInfo
	                        ActivationInfo, bool bReplicateEndAbility
	                        , bool bWasCancelled) override;

private:
	uint32 SelectedFoodRecipeId;

	UPROPERTY()
	TObjectPtr<UAT_PlayCinematic> AT_PlayCinematic;

	UPROPERTY()
	TObjectPtr<UAT_LogWithScreenShot> AT_LogCooking;

	UPROPERTY(EditDefaultsOnly, Category = "Options"
		, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<ULevelSequence>> QTESequenceList;

	TQueue<TObjectPtr<ULevelSequence>> RemainQTEQueue;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnPlayReadySequence(ALevelSequenceActor* TargetSequenceActor);

	UFUNCTION()
	void OnEndSequence();

	void PlayNextSequence();

	void LoggingToStartCook();
	void LoggingToEndCook();
};
