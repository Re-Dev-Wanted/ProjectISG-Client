#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingQTEUI.generated.h"

enum class ECookingQTEType : uint8;

UCLASS()
class PROJECTISG_API UUIC_CookingQTEUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void StartQTE(const ECookingQTEType QTEType);
};
