#pragma once
#include "CookingEnum.h"
#include "ProjectISG/Utils/MacroUtil.h"

#include "CookingStruct.generated.h"

class ULevelSequence;
enum class ECookingTool : uint8;

USTRUCT(BlueprintType)
struct PROJECTISG_API FCookingRecipeFlow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint16> RecipeFlow;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FFoodRecipe : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(uint32, FoodId)
	GETTER(ECookingTool, CookingTool)
	FORCEINLINE TMap<uint16, uint8> GetRecipeData() const { return RecipeData; }

private:
	// 실제로는 Item Table에 들어가는 아이디로 Recipe의
	// 고유한 RowName과 별개로 관리한다.
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint32 FoodId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TMap<uint16, uint8> RecipeData;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	ECookingTool CookingTool = ECookingTool::None;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FEquipCookingToolToActParams
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* AttachPoint = nullptr;

	UPROPERTY()
	ECookingTool CookingTool = ECookingTool::None;
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FCookingQTESequence
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelSequence> Sequence;

	UPROPERTY(EditDefaultsOnly)
	ECookingTool CookingTool = ECookingTool::None;
};
