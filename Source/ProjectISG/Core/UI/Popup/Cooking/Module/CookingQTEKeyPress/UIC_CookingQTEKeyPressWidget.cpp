#include "UIC_CookingQTEKeyPressWidget.h"

#include "UIC_CookingQTEKeyWidget.h"
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

	const uint8 RandomCount = FMath::RandRange(DataModel->GetMinAccessValue()
												, DataModel->
												GetMaxAccessValue());
	DataModel->SetCurrentQTEIndex(0);

	const TArray<ECookingQTEKey> ValidKeys = {
		ECookingQTEKey::Q, ECookingQTEKey::W, ECookingQTEKey::E
		, ECookingQTEKey::A, ECookingQTEKey::S, ECookingQTEKey::D
	};

	UIView->GetKeySlots()->ClearChildren();

	TArray<ECookingQTEKey> NewKeys;
	TArray<TObjectPtr<UUIV_CookingQTEKeyWidget>> NewWidgets;
	for (int i = 0; i < RandomCount; i++)
	{
		const int32 RandomIndex = UKismetMathLibrary::RandomInteger(
			ValidKeys.Num());

		UUIV_CookingQTEKeyWidget* NewKeyWidget = CreateWidget<
			UUIV_CookingQTEKeyWidget>(UIView, UIView->GetQTEKeyWidgetClass());
		UIView->GetKeySlots()->AddChild(NewKeyWidget);

		Cast<UUIC_CookingQTEKeyWidget>(NewKeyWidget->GetController())->
			SetupKeyPress(ValidKeys[RandomIndex]);

		NewKeys.Add(ValidKeys[RandomIndex]);
		NewWidgets.Add(NewKeyWidget);
	}

	DataModel->SetRemainQTEKeys(NewKeys);
	DataModel->SetKeyWidgets(NewWidgets);
}

void UUIC_CookingQTEKeyPressWidget::CheckQTE(const uint8 CookingQTEKey)
{
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());

	const bool IsSuccess = CookingQTEKey == static_cast<uint8>(DataModel->
		GetRemainQTEKeys()[DataModel->GetCurrentQTEIndex()]);

	Cast<UUIC_CookingQTEKeyWidget>(
		DataModel->GetKeyWidgets()[DataModel->GetCurrentQTEIndex()]->
		GetController())->PressToEnd(IsSuccess);

	// Key와 현재가 다른 경우에 대한 처리
	if (!IsSuccess)
	{
		UE_LOG(LogTemp, Display, TEXT("틀렸음"))
		return;
	}

	DataModel->SetCurrentQTEIndex(DataModel->GetCurrentQTEIndex() + 1);
	UE_LOG(LogTemp, Display, TEXT("맞았음"))
}
