﻿#include "GA_CookingQTEAction.h"

#include "LevelSequenceActor.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Contents/Cooking/Managers/CookingManager.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingQTE/UIC_CookingQTEUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIC_CookingRecipeUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingRecipe/UIM_CookingRecipeUI.h"
#include "ProjectISG/GAS/Common/Ability/Utility/AT_LogWithScreenShot.h"
#include "ProjectISG/GAS/Common/Ability/Utility/AT_PlayCinematic.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

void UGA_CookingQTEAction::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AT_LogCooking = UAT_LogWithScreenShot::InitialEvent(this);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	const AMainPlayerController* PC = Player->GetController<
		AMainPlayerController>();

	UUIC_CookingRecipeUI* CookingRecipeController = Cast<UUIC_CookingRecipeUI>(
		PC->GetUIManageComponent()->ControllerInstances[
			EUIName::Popup_CookingRecipeUI]);

	// 현재 UI에서 선택한 Recipe Id 가져오기
	SelectedFoodRecipeId = Cast<UUIM_CookingRecipeUI>(
		CookingRecipeController->GetModel())->GetSelectedRecipe();

	// 그다음 Push UI로 다음 Flow UI로 넘어가기
	Player->GetController<AMainPlayerController>()->PushUI(
		EUIName::Popup_CookingQTE);

	RemainQTEQueue.Empty();
	for (int i = 0; i < QTESequenceList.Num(); i++)
	{
		RemainQTEQueue.Enqueue(QTESequenceList[i]);
	}

	PlayNextSequence();

	LoggingToStartCook();
}

void UGA_CookingQTEAction::PlayNextSequence()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	// 새로운 시네마틱 진행 시 QTE 진행 화면 가리기
	// 비즈니스 로직 상 모든 QTE는 시네마틱 시작 쯤부터 진행해
	// 시네마틱 1개 종료 시점에 반드시 마무리 해야 함을 명시한다.
	Cast<UUIC_CookingQTEUI>(
		Player->GetController<AMainPlayerController>()->GetUIManageComponent()->
				ControllerInstances[EUIName::Popup_CookingQTE])->SetHiddenQTE();

	AT_PlayCinematic = UAT_PlayCinematic::InitialEvent(
		this, RemainQTEQueue.Peek()->Sequence, LevelSequenceActor);

	AT_PlayCinematic->OnPlayCinematicOnReadyNotified.Clear();
	AT_PlayCinematic->OnPlayCinematicOnReadyNotified.AddDynamic(
		this, &ThisClass::OnPlayReadySequence);
	AT_PlayCinematic->OnPlayCinematicEndNotified.Clear();
	AT_PlayCinematic->OnPlayCinematicEndNotified.AddDynamic(
		this, &ThisClass::OnEndSequence);

	AT_PlayCinematic->ReadyForActivation();
}

void UGA_CookingQTEAction::OnPlayReadySequence(
	ALevelSequenceActor* TargetSequenceActor)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());
	TargetSequenceActor->AddBindingByTag(FName(TEXT("Player")), Player);

	AKitchenFurniture* KitchenFurniture = Cast<AKitchenFurniture>(
		Player->GetInteractionComponent()->GetTargetTraceResult().GetActor());

	KitchenFurniture->EquipCookingToolToAct({
		Player->GetMesh(), RemainQTEQueue.Peek()->CookingTool
	});
}

void UGA_CookingQTEAction::EndAbility(const FGameplayAbilitySpecHandle Handle
									, const FGameplayAbilityActorInfo* ActorInfo
									, const FGameplayAbilityActivationInfo
									ActivationInfo, bool bReplicateEndAbility
									, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
					, bWasCancelled);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());
	if (!Player)
	{
		return;
	}

	if (!Player->GetController<AMainPlayerController>())
	{
		return;
	}

	LoggingToEndCook();

	Player->GetController<AMainPlayerController>()->PopUI();

	// 완료된 음식 아이템 주기
	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[
		SelectedFoodRecipeId];
	const FItemMetaInfo NewFoodItem = UItemManager::GetInitialItemMetaDataById(
		Recipe.GetFoodId());

	Player->GetPlayerState<AMainPlayerState>()->GetInventoryComponent()->
			AddItem(NewFoodItem);
}

void UGA_CookingQTEAction::OnEndSequence()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	AKitchenFurniture* KitchenFurniture = Cast<AKitchenFurniture>(
		Player->GetInteractionComponent()->GetTargetTraceResult().GetActor());

	KitchenFurniture->UnEquipCookingToolToAct();

	RemainQTEQueue.Pop();
	if (RemainQTEQueue.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo
					, true, false);
		return;
	}

	PlayNextSequence();
}

void UGA_CookingQTEAction::LoggingToStartCook()
{
	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[
		SelectedFoodRecipeId];

	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("요리장");
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::start_cooking;
	LogParams.Detail = UItemManager::GetItemInfoById(Recipe.GetFoodId()).
		GetDisplayName() + TEXT(" ") + TEXT("1개");

	AT_LogCooking->SetLogParams(LogParams);
	AT_LogCooking->SetIsLogWithScreenShot(false);

	AT_LogCooking->Activate();
}

void UGA_CookingQTEAction::LoggingToEndCook()
{
	const FFoodRecipe Recipe = UCookingManager::GetRecipeData()[
		SelectedFoodRecipeId];

	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("요리장");
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::finish_cooking;
	LogParams.Detail = UItemManager::GetItemInfoById(Recipe.GetFoodId()).
		GetDisplayName() + TEXT(" ") + TEXT("1개");

	AT_LogCooking->SetLogParams(LogParams);
	AT_LogCooking->SetIsLogWithScreenShot(true);

	AT_LogCooking->Activate();
}
