#include "ISGAbilitySystemComponent.h"
#include "ISGAbilitySystemInitializeData.h"
#include "Ability/Utility/BaseInputAbility.h"


UISGAbilitySystemComponent::UISGAbilitySystemComponent()
{
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	SetIsReplicatedByDefault(true);
}


void UISGAbilitySystemComponent::Initialize(const UISGAbilitySystemInitializeData* InitialData)
{
	if (GetIsInitialize())
	{
		return;
	}
	
	if (!InitialData->GetDefaultGameplayAbilities().IsEmpty())
	{
		for (auto Ability : InitialData->GetDefaultGameplayAbilities())
		{
			const UBaseInputAbility* InputAbility = Ability->GetDefaultObject<UBaseInputAbility>();
			const int32 InputId = InputAbility->GetInputId() == EAbilityInputId::Undefined
				? INDEX_NONE : static_cast<int32>(InputAbility->GetInputId());
			
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
