#include "UIC_LootContainerUI.h"

#include "UIV_LootContainerUI.h"
#include "Components/Button.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"

void UUIC_LootContainerUI::InitializeController(UBaseUIView* NewView,
                                                UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(NewView);

	UIView->GetBackButton()->OnClicked.AddDynamic(this,
	                                              &UUIC_LootContainerUI::
	                                              CloseUI);
}

void UUIC_LootContainerUI::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started,
	                           this, &ThisClass::CloseUI);
}

void UUIC_LootContainerUI::CloseUI()
{
	ResetUIFromPlayerController();

	if (UIHandler)
	{
		UIHandler->OnClosed();
	}
}

void UUIC_LootContainerUI::SetContainer(FGuid Guid,
                                        const TArray<FItemMetaInfo>& Items,
                                        TScriptInterface<IItemHandler> Handler)
{
	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(GetView());

	if (Handler.GetObject()->GetClass()->ImplementsInterface
		(UUIHandler::StaticClass()))
	{
		TScriptInterface<IUIHandler> _Handler;
		_Handler.SetObject(Handler.GetObject());
		_Handler.SetInterface(Cast<IUIHandler>(Handler.GetObject()));

		UIHandler = _Handler;
	}

	UIView->SetContainer(Guid, Items, Handler);
}
