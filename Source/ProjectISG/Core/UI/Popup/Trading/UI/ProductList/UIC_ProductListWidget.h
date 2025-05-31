#pragma once

#include "CoreMinimal.h"
#include "UIV_ProductListWidget.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_ProductListWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_ProductListWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeData();
	
	void OnUpdateList(ETradingState State);

protected:
	void SetProductList();

	void SetInventoryList();
};
