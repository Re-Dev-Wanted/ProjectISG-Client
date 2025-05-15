#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "UIC_LootContainerUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_LootContainerUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetContainer(FGuid Guid, const TArray<FItemMetaInfo>& Items);

protected:
	virtual void InitializeController(UBaseUIView* NewView, UBaseUIModel* NewModel) override;

	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	UInputAction* CloseAction;
};
