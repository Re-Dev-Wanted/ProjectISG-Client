#include "UIC_LootContainerUI.h"

#include "UIV_LootContainerUI.h"
#include "Components/Button.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "UIM_LootContainerUI.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"

void UUIC_LootContainerUI::InitializeController(UBaseUIView* NewView,
                                                UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);
}

void UUIC_LootContainerUI::AppearUI()
{
	Super::AppearUI();
}

void UUIC_LootContainerUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started,
	                           this, &ThisClass::OnCloseUI);
}

void UUIC_LootContainerUI::OnCloseUI()
{
	UUIM_LootContainerUI* UIModel = Cast<UUIM_LootContainerUI>(GetModel());
	
	if (UIModel->GetUIHandler())
	{
		UIModel->GetUIHandler()->OnClosed();
	}
	
	ResetUIFromPlayerController();
}

void UUIC_LootContainerUI::SetContainer(const TArray<FItemMetaInfo>& Items,
	const TScriptInterface<IItemHandler>& Handler,
	const TScriptInterface<IUIHandler>& UIHandler)
{
	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(GetView());
	UUIM_LootContainerUI* UIModel = Cast<UUIM_LootContainerUI>(GetModel());

	UIModel->SetUIHandler(UIHandler);
	UIView->SetContainer(Items, Handler);
	UIView->GetBackButton()->OnClicked.AddUniqueDynamic(this,
		&ThisClass::OnCloseUI);
}
