#include "UIC_CookingRecipeUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void UUIC_CookingRecipeUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseCookingRecipeUI, ETriggerEvent::Triggered,
	                           this, &ThisClass::OnCloseCookingRecipeUI);
}

void UUIC_CookingRecipeUI::OnCloseCookingRecipeUI()
{
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Cooking_Active_EndCooking);

	GetPlayerController()->GetPlayerState<AMainPlayerState>()->
	                       GetAbilitySystemComponent()->
	                       TryActivateAbilitiesByTag(ActivateTag);
}
