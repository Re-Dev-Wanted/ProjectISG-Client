#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Ability/Utility/GA_BaseInputAbility.h"
#include "GA_CookingQTEAction.generated.h"

enum class EItemGrade : uint8;
enum class ECookingTool : uint8;

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
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CookingQTEWokCinematic;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CookingQTEFryingPanCinematic;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CookingQTEBigPotCinematic;
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> CookingQTEKnifeCinematic;

	uint32 SelectedFoodRecipeId;

	UPROPERTY()
	TObjectPtr<UAT_PlayCinematic> AT_PlayCinematic;

	UPROPERTY(EditDefaultsOnly, Category = "Options"
		, meta = (AllowPrivateAccess = true))
	TArray<ECookingTool> QTESequenceList;

	TQueue<ECookingTool> RemainQTEQueue;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnPlayReadySequence(ALevelSequenceActor* TargetSequenceActor);

	TObjectPtr<ULevelSequence> GetCookingSequence();

	UFUNCTION()
	void OnEndSequence();

	void PlayNextSequence();

	void LoggingToStartCook();
	void LoggingToEndCook();

	EItemGrade GetResultFoodGrade();
};
