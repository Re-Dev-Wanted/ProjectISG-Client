#include "UIV_CookingQTEKeyPressWidget.h"

#include "UIC_CookingQTEKeyPressWidget.h"

void UUIV_CookingQTEKeyPressWidget::NativeTick(const FGeometry& MyGeometry
												, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetController())
	{
		Cast<UUIC_CookingQTEKeyPressWidget>(GetController())->DecreaseTime(
			InDeltaTime);
	}
}
