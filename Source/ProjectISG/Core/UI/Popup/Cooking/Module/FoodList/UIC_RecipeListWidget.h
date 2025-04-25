#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_RecipeListWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_RecipeListWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeData();
};
