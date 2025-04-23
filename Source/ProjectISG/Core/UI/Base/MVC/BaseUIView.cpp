#include "BaseUIView.h"

void UBaseUIView::InitializeUI(UBaseUIModel* BaseUIModel,
                               UBaseUIController* BaseUIController)
{
	Model = BaseUIModel;
	Controller = BaseUIController;
}
