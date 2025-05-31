#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_QuestShowItemInfoWidget.generated.h"

struct FQuestRewardData;

UCLASS()
class PROJECTISG_API UUIC_QuestShowItemInfoWidget : public UBaseUIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = Option)
	TSoftObjectPtr<UTexture2D> GoldIcon;

public:
	void SetShowItemInfo(FQuestRewardData& RewardData);
};
