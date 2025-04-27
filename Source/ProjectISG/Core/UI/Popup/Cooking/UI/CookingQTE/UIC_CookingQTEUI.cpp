#include "UIC_CookingQTEUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_CookingQTEUI.h"
#include "UIV_CookingQTEUI.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/CookingQTEKeyPress/UIC_CookingQTEKeyPressWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/CookingQTEKeyPress/UIV_CookingQTEKeyPressWidget.h"

void UUIC_CookingQTEUI::StartQTE(const ECookingQTEType QTEType)
{
	UUIM_CookingQTEUI* QTEModel = Cast<UUIM_CookingQTEUI>(GetModel());
	QTEModel->SetCurrentQTEType(QTEType);
}

void UUIC_CookingQTEUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(QTEInputAction, ETriggerEvent::Triggered, this
								, &ThisClass::OnPressQTEAction);
}

void UUIC_CookingQTEUI::OnPressQTEAction(const FInputActionValue& Value)
{
	const UUIM_CookingQTEUI* QTEModel = Cast<UUIM_CookingQTEUI>(GetModel());
	if (QTEModel->GetCurrentQTEType() == ECookingQTEType::None)
	{
		return;
	}

	const UUIV_CookingQTEUI* QTEUI = Cast<UUIV_CookingQTEUI>(GetView());
	UUIC_CookingQTEKeyPressWidget* KeyPressQTEController = Cast<
		UUIC_CookingQTEKeyPressWidget>(
		QTEUI->GetQTEKeyPress()->GetController());

	const float ValueData = Value.Get<float>();

	KeyPressQTEController->CheckQTE(static_cast<uint8>(ValueData));
}
