#include "KitchenFurnitureQTEComponent.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIC_CookingRecipeUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIM_CookingRecipeUI.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

UKitchenFurnitureQTEComponent::UKitchenFurnitureQTEComponent()
{
	LevelSequenceActor = nullptr;
}

void UKitchenFurnitureQTEComponent::StartQTE()
{
	SetCurrentQTEIndex(0);
	PlayNextCookingQTE();
}

void UKitchenFurnitureQTEComponent::PlayNextCookingQTE()
{
	AKitchenFurniture* Kitchen = Cast<AKitchenFurniture>(GetOwner());
	AMainPlayerCharacter* Player = Kitchen->GetUsingCharacter();
	AMainPlayerController* PC = Player->GetController<AMainPlayerController>();

	PC->PushUI(EUIName::Popup_CookingQTE);
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


	if (GetCurrentQTEIndex() == CookingQTESequences.Num() - 1)
	{
		LevelSequenceActor->Destroy();
		return;
	}

	SetCurrentQTEIndex(GetCurrentQTEIndex() + 1);

	// 시네마틱 세팅 및 재생 준비
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), CookingQTESequences[GetCurrentQTEIndex()].Sequence
		, PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(
		this, &ThisClass::PlayNextCookingQTE);

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), Player);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")), Kitchen);

	LevelSequenceActor->SetActorTransform(Player->GetTransform());

	LevelSequencePlayer->Play();
}

// void UKitchenFurnitureQTEComponent::LoggingToStartCook()
// {
// 	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[
// 		SelectedFoodRecipeId];
//
// 	FDiaryLogParams LogParams;
// 	LogParams.Location = TEXT("요리장");
// 	LogParams.ActionType = ELoggingActionType::COOKING;
// 	LogParams.ActionName = ELoggingActionName::start_cooking;
// 	LogParams.Detail = UItemManager::GetItemInfoById(Recipe.GetFoodId()).
// 		GetDisplayName() + TEXT(" ") + TEXT("1개");
//
// 	AT_LogCooking->SetLogParams(LogParams);
// 	AT_LogCooking->SetIsLogWithScreenShot(false);
//
// 	AT_LogCooking->Activate();
// }
//
// void UKitchenFurnitureQTEComponent::LoggingToEndCook()
// {
// 	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[
// 		SelectedFoodRecipeId];
//
// 	FDiaryLogParams LogParams;
// 	LogParams.Location = TEXT("요리장");
// 	LogParams.ActionType = ELoggingActionType::COOKING;
// 	LogParams.ActionName = ELoggingActionName::finish_cooking;
// 	LogParams.Detail = UItemManager::GetItemInfoById(Recipe.GetFoodId()).
// 		GetDisplayName() + TEXT(" ") + TEXT("1개");
//
// 	AT_LogCooking->SetLogParams(LogParams);
// 	AT_LogCooking->SetIsLogWithScreenShot(true);
//
// 	AT_LogCooking->Activate();
// }
