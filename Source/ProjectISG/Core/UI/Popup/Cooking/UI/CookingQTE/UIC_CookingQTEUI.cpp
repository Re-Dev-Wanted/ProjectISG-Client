#include "UIC_CookingQTEUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_CookingQTEUI.h"
#include "UIV_CookingQTEUI.h"
#include "ProjectISG/Contents/Cooking/CookingEnum.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/CookingQTEKeyPress/UIC_CookingQTEKeyPressWidget.h"
#include "ProjectISG/Core/UI/Popup/Cooking/Module/CookingQTEKeyPress/UIV_CookingQTEKeyPressWidget.h"

void UUIC_CookingQTEUI::StartQTE(const ECookingQTEType QTEType)
{
	UUIM_CookingQTEUI* QTEModel = Cast<UUIM_CookingQTEUI>(GetModel());
	const UUIV_CookingQTEUI* QTEUI = Cast<UUIV_CookingQTEUI>(GetView());

	QTEModel->SetCurrentQTEType(QTEType);
	QTEModel->SetQTEStatus(ECookingQTEStatus::Trying);
	QTEModel->SetQTEScore(0);

	if (QTEType == ECookingQTEType::KeyPressArray)
	{
		Cast<UUIC_CookingQTEKeyPressWidget>(
			QTEUI->GetQTEKeyPress()->GetController())->StartQTE();
	}
}

void UUIC_CookingQTEUI::SetQTEStatus(const ECookingQTEStatus QTEStatus)
{
	UUIM_CookingQTEUI* QTEModel = Cast<UUIM_CookingQTEUI>(GetModel());
	QTEModel->SetQTEStatus(QTEStatus);
}

bool UUIC_CookingQTEUI::IsPlayerQTETrying()
{
	UUIM_CookingQTEUI* QTEModel = Cast<UUIM_CookingQTEUI>(GetModel());

	return QTEModel->GetQTEStatus() == ECookingQTEStatus::Trying;
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

	// QTE 타입이 현재 아무것도 없으면 굳이 처리 할 이유가 없다.
	if (QTEModel->GetCurrentQTEType() == ECookingQTEType::None)
	{
		return;
	}

	// QTE 상태가 진행중이 아니라면 역시 Input 처리가 필요하지 않다.
	if (QTEModel->GetQTEStatus() != ECookingQTEStatus::Trying)
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
