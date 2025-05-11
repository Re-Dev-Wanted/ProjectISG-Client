
#include "InteractiveItemUI.h"

#include "Components/TextBlock.h"

void UInteractiveItemUI::SetInteractive(const FString& Key,
	const FString& Text) const
{
	DisplayKey->SetText(FText::FromString(Key));
	DisplayText->SetText(FText::FromString(Text));
}
