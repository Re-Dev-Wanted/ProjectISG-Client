#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CurrentQuestWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_CurrentQuestWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void StartCurrentQuestAnimation();
};
