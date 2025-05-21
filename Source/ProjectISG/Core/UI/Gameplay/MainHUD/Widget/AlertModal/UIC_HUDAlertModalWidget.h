#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_HUDAlertModalWidget.generated.h"

enum class EAlertType : uint8;

UCLASS()
class PROJECTISG_API UUIC_HUDAlertModalWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void Alert(const EAlertType AlertType, const FString& Message,
	           const float Time = 2.f);

private:
	FTimerHandle AlertTimerHandle;
};
