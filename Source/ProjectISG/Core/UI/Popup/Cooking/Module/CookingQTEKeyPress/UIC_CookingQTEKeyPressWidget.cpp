#include "UIC_CookingQTEKeyPressWidget.h"

#include "UIC_CookingQTEKeyWidget.h"
#include "UIM_CookingQTEKeyPressWidget.h"
#include "UIV_CookingQTEKeyPressWidget.h"
#include "UIV_CookingQTEKeyWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectISG/Contents/Cooking/CookingEnum.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingQTE/UIC_CookingQTEUI.h"

void UUIC_CookingQTEKeyPressWidget::StartQTE()
{
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());
	UUIV_CookingQTEKeyPressWidget* UIView = Cast<UUIV_CookingQTEKeyPressWidget>(
		GetView());

	// 현재 QTE 키 정보들을 전부 비워준다. (새로 채워 새로운 미션 주기)
	DataModel->GetRemainQTEKeys().Empty();
	UIView->GetKeySlots()->ClearChildren();

	// 할당할 갯수는 랜덤으로 할당한다.
	const uint8 RandomCount = FMath::RandRange(DataModel->GetMinAccessValue()
												, DataModel->
												GetMaxAccessValue());
	DataModel->SetCurrentQTEIndex(0);
	DataModel->SetElapsedTime(0);

	// Key 입력 QTE는 해당 6개의 Key로 구성되어 있다.
	const TArray ValidKeys = {
		ECookingQTEKey::Q, ECookingQTEKey::W, ECookingQTEKey::E
		, ECookingQTEKey::A, ECookingQTEKey::S, ECookingQTEKey::D
	};

	// 새로운 키 정보 할당을 위해 새로운 배열을 만들어 값을 먼저 넣어준다.
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

	// 새로 만든 배열을 복사해서 넣어준다.
	DataModel->SetRemainQTEKeys(NewKeys);
	DataModel->SetKeyWidgets(NewWidgets);
}

void UUIC_CookingQTEKeyPressWidget::CheckQTE(const uint8 CookingQTEKey)
{
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());

	// 검사할 Index가 만약 배열을 넘어서면 검사하면 안된다.
	if (DataModel->GetCurrentQTEIndex() >= DataModel->GetRemainQTEKeys().Num())
	{
		return;
	}

	const ECookingQTEKey InputKey = DataModel->GetRemainQTEKeys()[DataModel->
		GetCurrentQTEIndex()];
	const bool IsSuccess = CookingQTEKey == static_cast<uint8>(InputKey);

	Cast<UUIC_CookingQTEKeyWidget>(
		DataModel->GetKeyWidgets()[DataModel->GetCurrentQTEIndex()]->
		GetController())->PressToEnd(IsSuccess);

	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());
	UBaseUIController* QTEController = PC->GetUIManageComponent()->
											ControllerInstances[
		EUIName::Popup_CookingQTE];


	// QTE 입력에 틀린 경우에 대한 처리
	if (!IsSuccess)
	{
		// TODO: 더 이상의 입력을 막아둔다.
		UE_LOG(LogTemp, Display, TEXT("틀렸음"))
		Cast<UUIC_CookingQTEUI>(QTEController)->SetQTEStatus(
			ECookingQTEStatus::Fail);
		return;
	}

	// 다음 Index로 이동하기 위해 우선 Index 값을 추가한다.
	DataModel->SetCurrentQTEIndex(DataModel->GetCurrentQTEIndex() + 1);
	if (DataModel->GetCurrentQTEIndex() >= DataModel->GetRemainQTEKeys().Num())
	{
		Cast<UUIC_CookingQTEUI>(QTEController)->SetQTEStatus(
			ECookingQTEStatus::Success);
	}
}

void UUIC_CookingQTEKeyPressWidget::DecreaseTime(const float DeltaTime)
{
	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		GetView()->GetOwningPlayer());
	UBaseUIController* QTEController = PC->GetUIManageComponent()->
											ControllerInstances[
		EUIName::Popup_CookingQTE];

	// QTE가 진행되지 않고 있다면 굳이 처리할 필요가 없다.
	if (!Cast<UUIC_CookingQTEUI>(QTEController)->IsPlayerQTETrying())
	{
		return;
	}

	const UUIV_CookingQTEKeyPressWidget* UIView = Cast<
		UUIV_CookingQTEKeyPressWidget>(GetView());
	UUIM_CookingQTEKeyPressWidget* DataModel = Cast<
		UUIM_CookingQTEKeyPressWidget>(GetModel());

	// 우선 현재 진행도를 변경한다.
	DataModel->SetElapsedTime(DataModel->GetElapsedTime() + DeltaTime);

	const float RemainPercent = 1 - FMath::Clamp(
		DataModel->GetElapsedTime() / DataModel->GetRemainTime(), 0.0f, 1.0f);
	UIView->GetRemainTimeBar()->SetPercent(RemainPercent);
}
