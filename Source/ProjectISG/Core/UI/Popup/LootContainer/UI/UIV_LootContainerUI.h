// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Module/UI_EscapeButton.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "UIV_LootContainerUI.generated.h"

class ULootContainerItemSlot;
class ULootContainerInventoryList;
class UButton;
class UTextBlock;
class ULootContainerListView;

UCLASS()
class PROJECTISG_API UUIV_LootContainerUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	TObjectPtr<UButton> GetBackButton() const
	{
		return BackButton->GetButton();
	}
	
	void SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items, TScriptInterface<IItemHandler> Handler) const;

protected:
	UPROPERTY(EditAnywhere, Category = "Options|UI",
			meta = (AllowPrivateAccess))
	TSubclassOf<ULootContainerItemSlot> InventorySlotClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULootContainerListView> ContainerView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULootContainerInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULootContainerInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_EscapeButton> BackButton;
};
