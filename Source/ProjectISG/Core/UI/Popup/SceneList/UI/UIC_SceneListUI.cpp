#include "UIC_SceneListUI.h"

#include "EnhancedInputComponent.h"
#include "UIM_SceneListUI.h"
#include "UIV_SceneListUI.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"

void UUIC_SceneListUI::InitializeController(UBaseUIView* NewView,
                                            UBaseUIModel* NewModel)
{
	Super::InitializeController(NewView, NewModel);
}

void UUIC_SceneListUI::BindInputAction(UEnhancedInputComponent* InputComponent)
{
	Super::BindInputAction(InputComponent);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Triggered, this,
	                           &ThisClass::OnTriggerSkipSceneAction);

	InputComponent->BindAction(SkipSceneAction, ETriggerEvent::Completed, this,
	                           &ThisClass::OnEndSkipSceneAction);
}

void UUIC_SceneListUI::OnTriggerSkipSceneAction()
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());
	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Visible);
	SceneListModel->SetCurrentLoadingPercent(
		SceneListModel->GetCurrentLoadingPercent() + 1);

	const float CurrentPercent = SceneListModel->
		GetCurrentLoadingPercent();

	SetSkipCircularPercent(
		CurrentPercent / SceneListModel->GetMaxLoadingPercent());

	if (SceneListModel->GetCurrentLoadingPercent() >= SceneListModel->
		GetMaxLoadingPercent())
	{
		GetView()->GetOwningPlayer<AMainPlayerController>()->PopUI();
		// TODO: 추후 행동에 대한 Delegate를 받아도 상관없을 것 같다
	}
}

void UUIC_SceneListUI::OnEndSkipSceneAction()
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());
	UUIM_SceneListUI* SceneListModel = Cast<UUIM_SceneListUI>(GetModel());

	SceneListModel->SetCurrentLoadingPercent(0);
	SceneListView->GetSkipLoadingWidget()->SetVisibility(
		ESlateVisibility::Hidden);
}

void UUIC_SceneListUI::SetSkipCircularPercent(const float Percent)
{
	const UUIV_SceneListUI* SceneListView = Cast<UUIV_SceneListUI>(GetView());

	SceneListView->GetCircularLoadingWidget()->GetDynamicMaterial()->
	               SetScalarParameterValue(FName("Percent"), Percent);
}
