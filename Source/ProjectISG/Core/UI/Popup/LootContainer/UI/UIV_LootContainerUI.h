// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_LootContainerUI.generated.h"

class UInventorySlot;
class UButton;
class UTextBlock;
class UInventoryList;
class ULootContainerListView;

UCLASS()
class PROJECTISG_API UUIV_LootContainerUI : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, BackButton)
	
	void Construct(FGuid Guid, int32 Capacity);

protected:
	UPROPERTY(EditAnywhere, Category = "Options|UI",
			meta = (AllowPrivateAccess))
	TSubclassOf<UInventorySlot> InventorySlotClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULootContainerListView> ContainerView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> MainSlotList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GuidText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;
};
