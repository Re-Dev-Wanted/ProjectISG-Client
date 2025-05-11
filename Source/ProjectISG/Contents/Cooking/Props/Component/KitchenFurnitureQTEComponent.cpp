#include "KitchenFurnitureQTEComponent.h"

#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIC_CookingRecipeUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIM_CookingRecipeUI.h"

UKitchenFurnitureQTEComponent::UKitchenFurnitureQTEComponent()
{
}

void UKitchenFurnitureQTEComponent::StartQTE()
{
	AKitchenFurniture* Kitchen = Cast<AKitchenFurniture>(GetOwner());
	AMainPlayerCharacter* Player = Kitchen->GetUsingCharacter();
	const AMainPlayerController* PC = Player->GetController<
		AMainPlayerController>();

	// TODO: UI를 통해 데이터를 가져오는 것 보다, UI에서 이 주방의 데이터를 설정해주는
	// 것이 더 좋아보인다. 현재는 UI에서 바로 가져오는 것으로 설정
	UUIC_CookingRecipeUI* CookingUIController = Cast<UUIC_CookingRecipeUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_CookingRecipeUI]);

	const UUIM_CookingRecipeUI* CookingUIModel = Cast<UUIM_CookingRecipeUI>(
		CookingUIController->GetModel());

	TArray<FCookingQTESequence> CookingQTESequences =
		UCookingManager::GetRecipeData()[CookingUIModel->GetSelectedRecipe()].
		GetQTESequences();

	SetCurrentQTEIndex(0);

	Kitchen->PlayCookingCinematic(
		Player, EKitchenFurnitureCinematicStatus::QTEAction);
}

void UKitchenFurnitureQTEComponent::PlayNextCookingQTE()
{
}
