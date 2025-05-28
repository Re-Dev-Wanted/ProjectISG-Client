#pragma once

#include <ProjectISG/Core/UI/Base/Module/UI_EscapeButton.h>

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Module/UI_BaseButton.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_ProductBuyNotification.generated.h"

class UUI_EscapeButton;
class UUI_BaseButton;
class USlider;
class UImage;
class UButton;
class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_ProductBuyNotification : public UBaseUIView
{
	GENERATED_BODY()

public:
	TObjectPtr<UButton> GetButton() const
	{
		return Button->Get();
	}

	TObjectPtr<UButton> GetCloseButton() const
	{
		return CloseButton->GetButton();
	}

	void OnInitialize();
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Increase();

	UFUNCTION()
	void Decrease();

	UFUNCTION()
	void OnCountValueChanged(float Value);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Dec_Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Inc_Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> CountSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_BaseButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_EscapeButton> CloseButton;
};
