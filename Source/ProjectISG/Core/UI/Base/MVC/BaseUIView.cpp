#include "BaseUIView.h"

#include "BaseUIController.h"
#include "BaseUIModel.h"

void UBaseUIView::NativeConstruct()
{
	Super::NativeConstruct();

	// 일단 View가 생성되는 시점에서 Model과 Controller를 반영한다.
	// MVC 패턴 특성상 Model을 직접 가져오는 행동은 최대한 자제한다.
	Model = NewObject<UBaseUIModel>(ModelClass);
	Controller = NewObject<UBaseUIController>(ControllerClass);

	Controller->InitializeController(this, Model);
}
