#include "ISGAbilitySystemComponent.h"
#include "ISGAbilitySystemInitializeData.h"
#include "Ability/Utility/GA_BaseInputAbility.h"

UISGAbilitySystemComponent::UISGAbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	SetIsReplicatedByDefault(true);
}


void UISGAbilitySystemComponent::Initialize(
	const UISGAbilitySystemInitializeData* InitialData)
{
	if (GetIsInitialize())
	{
		return;
	}

	if (!InitialData->GetDefaultGameplayAbilities().IsEmpty())
	{
		for (auto Ability : InitialData->GetDefaultGameplayAbilities())
		{
			const UGA_BaseInputAbility* InputAbility = Ability->GetDefaultObject
				<UGA_BaseInputAbility>();
			const int32 InputId = InputAbility->GetInputId() ==
			                      EAbilityInputId::Undefined
				                      ? INDEX_NONE
				                      : static_cast<int32>(InputAbility->
					                      GetInputId());
			
			GiveAbility(FGameplayAbilitySpec(
				Ability, InputAbility->GetAbilityLevel()
				, InputId, this));
		}
	}

	if (!InitialData->GetDefaultGameplayTags().IsEmpty())
	{
		AddLooseGameplayTags(InitialData->GetDefaultGameplayTags());
	}

	SetIsInitialize(true);
}

// void UISGAbilitySystemComponent::AbilityActorInfoSet()
// {
// 	OnGameplayEffectAppliedDelegateToSelf.AddUObject(
// 		this, &UISGAbilitySystemComponent::EffectApplied);
// }

// void UISGAbilitySystemComponent::AddCharacterAbilities(
// 	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
// {
// 	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
// 	{
// 		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
// 		//GiveAbility(AbilitySpec);
// 		GiveAbilityAndActivateOnce(AbilitySpec);
// 	}
// }
//
// void UISGAbilitySystemComponent::EffectApplied(
// 	UAbilitySystemComponent* AbilitySystemComponent,
// 	const FGameplayEffectSpec& EffectSpec,
// 	FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
// {
// 	FGameplayTagContainer TagContainer;
// 	EffectSpec.GetAllAssetTags(TagContainer);
//
// 	EffectAssetTags.Broadcast(TagContainer);
// }
