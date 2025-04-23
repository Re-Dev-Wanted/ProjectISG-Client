#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_InventoryUI.generated.h"

UCLASS()
class PROJECTISG_API UUIC_InventoryUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void UpdateMainSlotItemData() const;

	void UpdateInventorySlotItemData() const;
};
