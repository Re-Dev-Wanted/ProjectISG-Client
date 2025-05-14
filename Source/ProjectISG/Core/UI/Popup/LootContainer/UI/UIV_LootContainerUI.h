// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
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
	GETTER(TObjectPtr<UButton>, BackButton)
	
	void SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items) const;

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
	TObjectPtr<UTextBlock> GuidText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
};
