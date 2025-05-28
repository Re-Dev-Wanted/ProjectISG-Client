#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_ProductBuyNotification.generated.h"

UCLASS()
class PROJECTISG_API UUIC_ProductBuyNotification : public UBaseUIController
{
	GENERATED_BODY()

public:
	void UpdateCount(int Count);
	
	void OnInitialize();

protected:
	virtual void InitializeController(UBaseUIView* NewView,
	                                  UBaseUIModel* NewModel) override;

	virtual void AppearUI() override;

	UFUNCTION()
	void OnClickedButton();

private:
	void Logging();
};
