#include "UIC_DiaryEditUI.h"

#include "UIV_DiaryEditUI.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

void UUIC_DiaryEditUI::InitializeDiaryToEdit(
	const FGenerateDiaryResponse& Diary)
{
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(), ATimeManager::StaticClass()));

	const UUIV_DiaryEditUI* DiaryEditUIView = Cast<UUIV_DiaryEditUI>(GetView());

	DiaryEditUIView->GetDiaryDayText()->SetText(
		FText::FromString(FString::FromInt(TimeManager->GetTotalPlayingDay())));
	DiaryEditUIView->GetDiaryDescription()->SetText(
		FText::FromString(Diary.diary));
}
