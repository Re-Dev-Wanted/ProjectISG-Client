#include "UI_BaseButton.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"

void UUI_BaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text->SetText(TextValue);
}
