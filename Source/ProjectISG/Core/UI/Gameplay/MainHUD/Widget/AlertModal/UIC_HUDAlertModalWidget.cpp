#include "UIC_HUDAlertModalWidget.h"

#include "UIV_HUDAlertModalWidget.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"

void UUIC_HUDAlertModalWidget::Alert(const EAlertType AlertType,
                                     const FString& Message, const float Time)
{
	UUIV_HUDAlertModalWidget* AlertModalWidget = Cast<UUIV_HUDAlertModalWidget>(
		GetView());

	StartShowUI(EUILayer::Gameplay);

	AlertModalWidget->GetAlertMessage()->SetText(FText::FromString(Message));
	AlertModalWidget->GetAlertMessage()->SetColorAndOpacity(
		AlertModalWidget->GetColorMap()[AlertType]);

	TWeakObjectPtr<ThisClass> WeakThis = this;
	GetWorld()->GetTimerManager().ClearTimer(AlertTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AlertTimerHandle,
	                                       [WeakThis]()
	                                       {
	                                       		if (WeakThis.IsValid())
	                                       		{
													WeakThis.Get()->EndShowUI(true);	
	                                       		}
	                                       }, Time, false);
}
