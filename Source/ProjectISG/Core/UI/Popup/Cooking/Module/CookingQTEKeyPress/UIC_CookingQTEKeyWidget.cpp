#include "UIC_CookingQTEKeyWidget.h"

#include "UIV_CookingQTEKeyWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UUIC_CookingQTEKeyWidget::SetupKeyPress(const ECookingQTEKey KeyValue)
{
	const UUIV_CookingQTEKeyWidget* PressKeyView = Cast<
		UUIV_CookingQTEKeyWidget>(GetView());
	PressKeyView->GetPressKey()->SetText(
		FText::FromString(FEnumUtil::GetClassEnumKeyAsString(KeyValue)));
}

void UUIC_CookingQTEKeyWidget::PressToEnd(const bool IsSuccess)
{
	const UUIV_CookingQTEKeyWidget* PressKeyView = Cast<
		UUIV_CookingQTEKeyWidget>(GetView());

	if (IsSuccess)
	{
		PressKeyView->GetIsDoneBorder()->SetVisibility(
			ESlateVisibility::Visible);
		PressKeyView->GetIsMissBorder()->SetVisibility(
			ESlateVisibility::Hidden);
	}
	else
	{
		PressKeyView->GetIsDoneBorder()->SetVisibility(
			ESlateVisibility::Hidden);
		PressKeyView->GetIsMissBorder()->SetVisibility(
			ESlateVisibility::Visible);
	}
}
