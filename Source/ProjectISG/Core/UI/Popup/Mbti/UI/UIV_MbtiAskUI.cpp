#include "UIV_MbtiAskUI.h"

#include "UIM_MbtiAskUI.h"
#include "Components/ProgressBar.h"

void UUIV_MbtiAskUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!GetController())
	{
		return;
	}

	UUIM_MbtiAskUI* MbtiAskUIModel = Cast<UUIM_MbtiAskUI>(GetModel());

	const float PercentValue = FMath::FInterpTo(
		MbtiAskUIModel->GetCurrentPercentValue(),
		MbtiAskUIModel->GetCurrentQuestNum(),
		InDeltaTime, 3.5f);

	MbtiAskUIModel->SetCurrentPercentValue(PercentValue);

	FlowProgressBar->SetPercent(
		MbtiAskUIModel->GetCurrentPercentValue() / static_cast<float>(
			MbtiAskUIModel->
			GetMaxQuestionNum()));
}
