#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "UIC_LootContainerUI.generated.h"

class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_LootContainerUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void SetContainer(const TArray<FItemMetaInfo>& Items, const TScriptInterface<IItemHandler>& Handler, const TScriptInterface<IUIHandler>& UIHandler);

protected:
	virtual void InitializeController(UBaseUIView* NewView, UBaseUIModel* NewModel) override;

	virtual void AppearUI() override;

	virtual void BindInputAction(UEnhancedInputComponent* InputComponent) override;

	UFUNCTION()
	void OnCloseUI();

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true, EditCondition = "IsInputAccess"))
	UInputAction* CloseAction;
};
