#include "UIC_CookingQTEKeyPressWidget.h"

#include "EnhancedInputComponent.h"
#include "UIM_CookingQTEKeyPressWidget.h"
#include "UIV_CookingQTEKeyPressWidget.h"
#include "UIV_CookingQTEKeyWidget.h"
#include "Components/HorizontalBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectISG/Contents/Cooking/CookingEnum.h"

void UUIC_CookingQTEKeyPressWidget::StartQTE()
{
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());
	UUIV_CookingQTEKeyPressWidget* UIView = Cast<UUIV_CookingQTEKeyPressWidget>(
		GetView());
	DataModel->GetRemainQTEKeys().Empty();

	const uint8 RandomCount = FMath::RandRange(4, 10);
	DataModel->SetMaxQTELength(RandomCount);
	DataModel->SetCurrentQTEIndex(0);

	const TArray<ECookingQTEKey> ValidKeys = {
		ECookingQTEKey::Q, ECookingQTEKey::W, ECookingQTEKey::E
		, ECookingQTEKey::A, ECookingQTEKey::S, ECookingQTEKey::D
	};

	for (int i = 0; i < RandomCount; i++)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomInteger(
			ValidKeys.Num());
		DataModel->GetRemainQTEKeys().Add(ValidKeys[RandomIndex]);

		UUIV_CookingQTEKeyWidget* NewKeyWidget = CreateWidget<
			UUIV_CookingQTEKeyWidget>(UIView, UIView->GetQTEKeyWidgetClass());
		UIView->GetKeySlots()->AddChild(NewKeyWidget);
	}
}

void UUIC_CookingQTEKeyPressWidget::BindInputAction(
	UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(QTEInputAction, ETriggerEvent::Triggered, this
								, &ThisClass::OnPressQTEAction);
}

void UUIC_CookingQTEKeyPressWidget::OnPressQTEAction(
	const FInputActionValue& Value)
{
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());
	const float ValueData = Value.Get<float>();

	// Key와 현재가 다른 경우에 대한 처리
	if (static_cast<uint8>(ValueData) != static_cast<uint8>(DataModel->
		GetRemainQTEKeys()[DataModel->GetCurrentQTEIndex()]))
	{
		UE_LOG(LogTemp, Display, TEXT("틀렸음"))
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("맞았음"))
	DataModel->SetCurrentQTEIndex(DataModel->GetCurrentQTEIndex() + 1);
}
