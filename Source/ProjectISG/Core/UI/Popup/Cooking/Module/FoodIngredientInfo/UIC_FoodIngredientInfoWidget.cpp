﻿#include "UIC_FoodIngredientInfoWidget.h"

#include "UIM_FoodIngredientInfoWidget.h"
#include "UIV_FoodIngredientInfoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"

void UUIC_FoodIngredientInfoWidget::SetIngredientInfo(const uint32 IngredientId,
	const uint32 IngredientCount)
{
	const UUIV_FoodIngredientInfoWidget* FoodInfoWidget = Cast<
		UUIV_FoodIngredientInfoWidget>(
		GetView());
	UUIM_FoodIngredientInfoWidget* FoodInfoModel = Cast<
		UUIM_FoodIngredientInfoWidget>(GetModel());

	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(
		IngredientId);
	UTexture2D* Thumbnail = ItemInfoData.GetThumbnail()
	                                    .LoadSynchronous();

	FoodInfoModel->SetIngredientId(IngredientId);
	FoodInfoModel->SetIngredientCount(IngredientCount);

	// 썸네일 지정
	FoodInfoWidget->GetIngredientThumbnail()->
	                SetBrushFromTexture(Thumbnail);

	// 요구 아이템에 대한 옵션 지정
	FoodInfoWidget->GetIngredientCount()->SetText(
		FText::FromString(FString::FromInt(IngredientCount)));

	// 현재 플레이어가 가지고 있는 아이템의 갯수를 표시
	const UInventoryComponent* PlayerInventory = FoodInfoWidget->
	                                             GetOwningPlayerState<
		                                             AMainPlayerState>()->
	                                             GetInventoryComponent();

	const uint32 CurrentIngredientCount = PlayerInventory->
	                                      GetCurrentRemainItemValue().
	                                      Contains(IngredientId)
		                                      ? PlayerInventory->
		                                      GetCurrentRemainItemValue()[
			                                      IngredientId]
		                                      : 0;

	FColor Color = CurrentIngredientCount < IngredientCount
		               ? FColor::Red
		               : FColor::White;

	FoodInfoWidget->GetCurrentIngredientCount()->SetText(
		FText::AsNumber(CurrentIngredientCount));

	FoodInfoWidget->GetCurrentIngredientCount()->SetColorAndOpacity(Color);
}
