#include "UIC_WorkbenchUI.h"

#include "EnhancedInputComponent.h"
#include "UIV_WorkbenchUI.h"
#include "Components/Button.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"

void UUIC_WorkbenchUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started, this, &ThisClass::CloseUI);

	UUIV_WorkbenchUI* WorkbenchView = Cast<UUIV_WorkbenchUI>(GetView());
	WorkbenchView->GetCloseButton()->OnClicked.AddDynamic(this, &ThisClass::CloseUI);
	WorkbenchView->GetCraftingButton()->Get()->OnClicked.AddDynamic(this, &ThisClass::StartCrafting);
}

void UUIC_WorkbenchUI::StartCrafting()
{
}

void UUIC_WorkbenchUI::CloseUI()
{
	if (UIHandler)
	{
		UIHandler->OnClosed();
	}

	PopUIFromPlayerController();
}
