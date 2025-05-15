#include "UIC_LootContainerUI.h"

#include "UIV_LootContainerUI.h"
#include "Components/Button.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"

void UUIC_LootContainerUI::InitializeController(UBaseUIView* NewView,
	UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(NewView);

	UIView->GetBackButton()->OnClicked.AddDynamic(this, 
	&UUIC_LootContainerUI::PopUIFromPlayerController);
}

void UUIC_LootContainerUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(CloseAction, ETriggerEvent::Started,
								   this, &ThisClass::PopUIFromPlayerController);
}

void UUIC_LootContainerUI::SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items)
{
	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(GetView());
	
	UIView->SetContainer(Guid, Items);
}
