#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestRequiredTextWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_QuestRequiredTextWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeData(const FString& QuestRequireDescription,
	                    const FString& QuestRequireStatus) const;
};
