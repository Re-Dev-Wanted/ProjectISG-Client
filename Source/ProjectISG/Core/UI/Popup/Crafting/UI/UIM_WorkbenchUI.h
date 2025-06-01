#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_WorkbenchUI.generated.h"

USTRUCT(BlueprintType)
struct FCraftingMaterialUIModel
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Id = 0;

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY()
	uint16 RequiredCount = 0;
};

USTRUCT(BlueprintType)
struct FCraftingRecipeUIModel
{
	GENERATED_BODY()

	UPROPERTY()
	uint16 Id = 0;

	UPROPERTY()
	uint16 ItemId = 0;

	UPROPERTY()
	FString ItemName;

	UPROPERTY()
	FText ItemDesc;

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY()
	TMap<uint16, FCraftingMaterialUIModel> RequiredMaterials;

	TArray<FCraftingMaterialUIModel> GetRequiredMaterialsArray() const
	{
		TArray<FCraftingMaterialUIModel> Array;
		if (RequiredMaterials.IsEmpty())
		{
			return Array;
		}

		RequiredMaterials.GenerateValueArray(Array);

		return Array;
	}
};

UCLASS()
class PROJECTISG_API UUIM_WorkbenchUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER(TArray<FCraftingRecipeUIModel>, Recipes)
	GETTER(uint16, SelectedRecipeId)

	void LoadAll();

	FCraftingRecipeUIModel GetRecipeUIModel(uint16 RecipeId) const;

private:
	TMap<uint16, FCraftingMaterialUIModel> LoadMaterialsModel(
		TMap<uint16, uint16> Map);

	TArray<FCraftingRecipeUIModel> Recipes;

	mutable uint16 SelectedRecipeId = 0;
};
