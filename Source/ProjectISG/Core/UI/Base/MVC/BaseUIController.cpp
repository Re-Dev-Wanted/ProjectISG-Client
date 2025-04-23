#include "BaseUIController.h"
#include "BaseUIView.h"

void UBaseUIController::AddViewToViewport(const uint16 ZIndex) const
{
	View->AddToViewport(ZIndex);
}

void UBaseUIController::InitializeController(UBaseUIView* NewView,
                                             UBaseUIModel* NewModel)
{
	View = NewView;
	Model = NewModel;
}
