#include "UIC_LootContainerUI.h"

#include "UIV_LootContainerUI.h"
#include "Components/Button.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

void UUIC_LootContainerUI::InitializeController(UBaseUIView* NewView,
	UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);

	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(NewView);

	UIView->GetBackButton()->OnClicked.AddDynamic(this, 
	&UUIC_LootContainerUI::CloseUI);
}

void UUIC_LootContainerUI::SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items)
{
	UUIV_LootContainerUI* UIView = Cast<UUIV_LootContainerUI>(GetView());
	
	UIView->SetContainer(Guid, Items);
}

void UUIC_LootContainerUI::CloseUI()
{
	if (const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetPlayerController()->GetPawn()))
	{
		AMainPlayerController* PC = Player->GetController<
			AMainPlayerController>();
		PC->PopUI();
	}
}
