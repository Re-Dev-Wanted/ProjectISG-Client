#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_CookingRecipeUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_CookingRecipeUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(int, SelectedRecipe)

private:
	// 일반적으로는 Recipe Id는 0으로 나오지만 선택된 레시피를 안나오게 처리하기 위해서
	// INDEX_NONE 이라는 값을 별도로 넣어둔다. 명시적으로 -1은 UI를 표시하지 않는다는 것을 전제하에 수행한다.
	UPROPERTY()
	int SelectedRecipe = INDEX_NONE;
};
