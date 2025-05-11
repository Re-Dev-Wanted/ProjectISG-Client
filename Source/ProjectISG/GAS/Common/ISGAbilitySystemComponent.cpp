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
	if (!InitialData)
	{
		return;
	}

	if (GetIsInitialize())
	{
		return;
	}

	// Ability는 반드시 서버 환경에서만 Give Ability를 수행해야 한다.
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (!InitialData->GetDefaultGameplayAbilities().IsEmpty())
		{
			for (auto Ability : InitialData->GetDefaultGameplayAbilities())
			{
				const UGA_BaseInputAbility* InputAbility = Ability->
					GetDefaultObject<UGA_BaseInputAbility>();
				const int32 InputId = InputAbility->GetInputId() ==
									EAbilityInputId::Undefined
										? INDEX_NONE
										: static_cast<int32>(InputAbility->
											GetInputId());

				GiveAbility(FGameplayAbilitySpec(
					Ability, InputAbility->GetAbilityLevel(), InputId, this));
			}
		}
	}

	if (!InitialData->GetDefaultGameplayTags().IsEmpty())
	{
		AddLooseGameplayTags(InitialData->GetDefaultGameplayTags());
	}

	SetIsInitialize(true);
}
