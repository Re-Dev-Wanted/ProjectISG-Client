// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "CraftingRecipe.generated.h"

USTRUCT(BlueprintType)
struct PROJECTISG_API FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(uint16, RecipeId);
	GETTER(uint16, ItemId);

	TMap<uint16, uint16> GetRequiredMaterials() const
	{
		return RequiredMaterials;
	}
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true)) // 이 레시피의 고유 id
	uint16 RecipeId = 0;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true)) // 이 레시피를 사용해 만들게 될 아이템 id
	uint16 ItemId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<uint16, uint16> RequiredMaterials; // 아이템을 만들기 위한 필요 재료 (key : itemId, value : count)
};
