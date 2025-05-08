#include "UIC_FishingUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "UIM_FishingUI.h"
#include "UIV_FishingUI.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/UI/HUD/Interactive/InteractiveItemUI.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void UUIC_FishingUI::SetUI(bool bIsActive, const FString& Key, const FString& Text)
{
	const UUIV_FishingUI* FishingView = Cast<UUIV_FishingUI>(GetView());
	FishingView->GetInteractiveUI()->SetInteractive(Key, Text);

	UUIM_FishingUI* FishingUIModel = Cast<UUIM_FishingUI>(GetModel());
	FishingUIModel->SetIsActive(bIsActive);
}

void UUIC_FishingUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CatchAction, ETriggerEvent::Started, 
	this, &ThisClass::OnAction);
}

void UUIC_FishingUI::OnAction()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>
	(GetPlayerController()->GetPawn());
	
	if (Player)
	{
		UUIM_FishingUI* FishingUIModel = Cast<UUIM_FishingUI>(GetModel());
		
		FGameplayEventData EventData;
		EventData.EventTag = ISGGameplayTags::Fishing_Active_ReelInLine;
		EventData.Instigator = Player;
		EventData.OptionalObject = FishingUIModel;
			
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(ISGGameplayTags::Fishing_Active_ReelInLine, &EventData);
	}
	
	PopUIFromPlayerController();
}
