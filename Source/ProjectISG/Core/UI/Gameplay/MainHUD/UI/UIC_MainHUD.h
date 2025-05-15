#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_MainHUD.generated.h"

UCLASS()
class PROJECTISG_API UUIC_MainHUD : public UBaseUIController
{
	GENERATED_BODY()

public:
	void UpdateMainHotSlot() const;

	void SelectSlot(const uint8 CurrentSlot, const uint8 NextSlot) const;

	void ToggleInteractiveUI(const bool IsShow);
	void ToggleInteractiveUI(const FString& DisplayKey,
	                         const FString& DisplayText);

	void AdditiveToggleInteractiveUI(const FString& DisplayKey,
	                                 const FString& DisplayText);

	void TogglePlacementIndicatorUI(const bool Visible);

	void StartAutoQuest(const FString& QuestId);

	void ToggleAutoQuestUI(const bool IsActive);

	void ToggleCurrentQuestUI(const bool IsActive);

protected:
	virtual void AppearUI() override;
};
