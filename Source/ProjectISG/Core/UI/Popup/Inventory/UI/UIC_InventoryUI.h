#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "UIC_InventoryUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_InventoryUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void UpdateMainSlotItemData() const;

	void UpdateInventorySlotItemData() const;

	void SetItemInfoData(const uint8 InventoryIndex);

	void ClearItemInfoData();

protected:
	virtual void AppearUI() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	TObjectPtr<UInputAction> CloseInventoryAction;

	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;
};
