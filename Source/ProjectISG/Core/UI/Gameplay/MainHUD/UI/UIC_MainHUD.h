#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_MainHUD.generated.h"

UCLASS()
class PROJECTISG_API UUIC_MainHUD : public UBaseUIController
{
	GENERATED_BODY()

public:
	virtual void AppearUI() override;

	void UpdateMainHotSlot() const;

	void SelectSlot(const uint8 CurrentSlot, const uint8 NextSlot) const;

	void InitializeHUD();

	void ToggleInteractiveUI(const bool IsShow);
	void ToggleInteractiveUI(const FString& DisplayKey,
	                         const FString& DisplayText);
};
