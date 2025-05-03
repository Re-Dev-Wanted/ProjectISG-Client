#include "UI_BaseButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProjectISG/Core/ISGGameInstance.h"

void UUI_BaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text->SetText(TextValue);
}
