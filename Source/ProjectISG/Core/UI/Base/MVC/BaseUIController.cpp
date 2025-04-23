#include "BaseUIController.h"

void UBaseUIController::InitializeController(UBaseUIView* NewView,
                                             UBaseUIModel* NewModel)
{
	View = NewView;
	Model = NewModel;
}
