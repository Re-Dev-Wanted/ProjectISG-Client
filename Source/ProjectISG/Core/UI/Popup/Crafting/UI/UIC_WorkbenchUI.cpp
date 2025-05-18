#include "UIC_WorkbenchUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_WorkbenchUI.h"
#include "UIV_WorkbenchUI.h"
#include "Components/Button.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"

void UUIC_WorkbenchUI::AppearUI()
{
	Super::AppearUI();
	
	UUIM_WorkbenchUI* WorkbenchModel = Cast<UUIM_WorkbenchUI>(GetModel());

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
		
	}
}

void UUIC_WorkbenchUI::OnUpdateSelectedRecipeUI(uint16 RecipeId)
{
	UUIM_WorkbenchUI* WorkbenchModel = Cast<UUIM_WorkbenchUI>(GetModel());
	UUIV_WorkbenchUI* WorkbenchView = Cast<UUIV_WorkbenchUI>(GetView());
	
	FCraftingRecipeUIModel UIModel = WorkbenchModel->GetRecipeUIModel(RecipeId);

	WorkbenchView->OnUpdateUI(UIModel);
	WorkbenchView->GetCraftingButton()->SetVisibility(ESlateVisibility::Visible);
}

void UUIC_WorkbenchUI::CloseUI()
{
	if (UIHandler)
	{
		UIHandler->OnClosed();
	}

	PopUIFromPlayerController();
}
