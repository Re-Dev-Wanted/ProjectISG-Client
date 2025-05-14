#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_LootContainerUI.generated.h"

UCLASS()
class PROJECTISG_API UUIC_LootContainerUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetUI(FGuid Guid, int32 Capacity);

	UFUNCTION()
	void CloseUI();

protected:
	virtual void InitializeController(UBaseUIView* NewView, UBaseUIModel* NewModel) override;
};
