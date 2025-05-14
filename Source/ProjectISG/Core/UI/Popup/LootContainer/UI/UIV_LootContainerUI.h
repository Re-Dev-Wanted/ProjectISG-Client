// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_LootContainerUI.generated.h"

class UInventoryList;
class ULootContainerListView;

UCLASS()
class PROJECTISG_API UUIV_LootContainerUI : public UBaseUIView
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULootContainerListView> ContainerView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryList> InventoryList;
};
