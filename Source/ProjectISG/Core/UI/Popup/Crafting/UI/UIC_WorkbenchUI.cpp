#include "UIC_WorkbenchUI.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "UIM_WorkbenchUI.h"
#include "UIV_WorkbenchUI.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_WorkbenchUI::AppearUI()
{
	Super::AppearUI();

	UUIM_WorkbenchUI* WorkbenchModel = Cast<UUIM_WorkbenchUI>(GetModel());
	
	if (IsInitialized)
	{
		OnUpdateSelectedRecipeUI(WorkbenchModel->GetSelectedRecipeId());
		return;
	}

	IsInitialized = true;

	WorkbenchModel->LoadAll();
	
	UUIV_WorkbenchUI* WorkbenchView = Cast<UUIV_WorkbenchUI>(GetView());
	
	WorkbenchView->GetCloseButton()->OnClicked.AddDynamic(this, &ThisClass::CloseUI);
	WorkbenchView->GetCraftingButton()->Get()->OnClicked.AddDynamic(this, &ThisClass::StartCrafting);
	WorkbenchView->GetCraftingButton()->SetVisibility(ESlateVisibility::Hidden);

	OnSelectedRecipe.BindUObject(this, &ThisClass::OnUpdateSelectedRecipeUI);
	
	WorkbenchView->OnSetup(WorkbenchModel->GetRecipes(), OnSelectedRecipe);
}

void UUIC_WorkbenchUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started, this, &ThisClass::CloseUI);
}

void UUIC_WorkbenchUI::StartCrafting()
{
	UUIM_WorkbenchUI* WorkbenchModel = Cast<UUIM_WorkbenchUI>(GetModel());

	uint16 RecipeId = WorkbenchModel->GetSelectedRecipeId();

	if (RecipeId > 0)
	{
		AMainPlayerState* PS = GetPlayerController()->GetPlayerState<AMainPlayerState>();

		FCraftingRecipeUIModel UIModel = WorkbenchModel->GetRecipeUIModel(RecipeId);

		for (FCraftingMaterialUIModel MaterialUIModel : UIModel.GetRequiredMaterialsArray())
		{
			PS->GetInventoryComponent()->RemoveItem(MaterialUIModel.Id, MaterialUIModel.RequiredCount);
		}

		FItemMetaInfo CraftedItemInfo = UItemManager::GetInitialItemMetaDataById(UIModel.ItemId);
		
		PS->GetInventoryComponent()->AddItem(CraftedItemInfo);
	}

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetPlayerController()->GetPawn());

	FGameplayEventData EventData;
	EventData.EventTag = ISGGameplayTags::Crafting_Active_OnCrafting;
	EventData.Instigator = Player;
	EventData.Target = Cast<AActor>(UIHandler.GetObject());
		
	Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);

	PopUIFromPlayerController();
}

void UUIC_WorkbenchUI::OnUpdateSelectedRecipeUI(uint16 RecipeId)
{
	UUIV_WorkbenchUI* WorkbenchView = Cast<UUIV_WorkbenchUI>(GetView());
	
	if (RecipeId <= 0)
	{
		WorkbenchView->GetCraftingButton()->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	AMainPlayerState* PS = GetPlayerController()->GetPlayerState<AMainPlayerState>();
	
	UUIM_WorkbenchUI* WorkbenchModel = Cast<UUIM_WorkbenchUI>(GetModel());
	
	FCraftingRecipeUIModel UIModel = WorkbenchModel->GetRecipeUIModel(RecipeId);

	TMap<uint16, uint16> OwningCounts;

	bool IsSatisfied = true;

	for (FCraftingMaterialUIModel MaterialUIModel : UIModel.GetRequiredMaterialsArray())
	{
		uint16 Count = PS->GetInventoryComponent()->GetCurrentCount(MaterialUIModel.Id);

		if (IsSatisfied && Count < MaterialUIModel.RequiredCount)
		{
			IsSatisfied = false;
		}
		
		OwningCounts.Add(MaterialUIModel.Id, Count);
	}

	WorkbenchView->GetCraftingButton()->SetIsEnabled(IsSatisfied);
	
	WorkbenchView->OnUpdateUI(UIModel, OwningCounts);
	WorkbenchView->GetCraftingButton()->SetVisibility(ESlateVisibility::Visible);
}

void UUIC_WorkbenchUI::CloseUI()
{
	const AMainPlayerController* PC =  Cast<AMainPlayerController>(GetPlayerController());

	if (PC)
	{
		PC->GetUIManageComponent()->ResetWidget();
	}
	else
	{
		PopUIFromPlayerController();
	}

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(GetPlayerController()->GetPawn());
	
	FGameplayEventData EventData;
	EventData.EventTag = ISGGameplayTags::Crafting_Active_EndCrafting;
	EventData.Instigator = Player;
	EventData.Target = Cast<AActor>(UIHandler.GetObject());
		
	Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
}
