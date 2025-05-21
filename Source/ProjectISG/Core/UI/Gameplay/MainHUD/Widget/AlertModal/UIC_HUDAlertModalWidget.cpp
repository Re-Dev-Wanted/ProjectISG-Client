#include "UIC_HUDAlertModalWidget.h"

#include "UIV_HUDAlertModalWidget.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"

void UUIC_HUDAlertModalWidget::Alert(const EAlertType AlertType,
                                     const FString& Message, const float Time)
{
	UUIV_HUDAlertModalWidget* AlertModalWidget = Cast<UUIV_HUDAlertModalWidget>(
		GetView());

	AlertModalWidget->SetVisibility(ESlateVisibility::Visible);
	AlertModalWidget->GetAlertMessage()->SetText(FText::FromString(Message));
	AlertModalWidget->GetAlertMessage()->SetColorAndOpacity(
		AlertModalWidget->GetColorMap()[AlertType]);

	GetWorld()->GetTimerManager().ClearTimer(AlertTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AlertTimerHandle,
	                                       [this]()
	                                       {
		                                       EndShowUI(true);
	                                       }, Time, false);
}
