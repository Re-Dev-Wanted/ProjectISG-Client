#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingQTEKeyWidget.generated.h"

enum class ECookingQTEKey : uint8;

UCLASS()
class PROJECTISG_API UUIC_CookingQTEKeyWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetupKeyPress(const ECookingQTEKey KeyValue);
	void PressToEnd(const bool IsSuccess);
};
