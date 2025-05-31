#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ItemInfoWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickedSelectedButton, uint16);

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnClickedSelectedButton OnClickedSelectedButton;

	void SetWidget(uint16 DataId, const FString& Name, FOnSelectedCraftingRecipe OnSelectedRecipe);

	void SetImage(UTexture2D* Image);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnExecuteClickEvent();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

private:
	uint16 Id = 0;
};
