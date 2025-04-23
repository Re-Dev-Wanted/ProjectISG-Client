#include "BaseUIController.h"

#include "BaseUIView.h"

void UBaseUIController::InitializeController(UBaseUIView* NewView,
                                             UBaseUIModel* NewModel)
{
	View = NewView;
	Model = NewModel;
}

void UBaseUIController::AppearUI()
{
	View->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UBaseUIController::DisappearUI()
{
	View->SetVisibility(ESlateVisibility::Hidden);
}
