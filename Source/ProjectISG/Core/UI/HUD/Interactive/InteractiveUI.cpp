#include "InteractiveUI.h"

#include "Components/TextBlock.h"

void UInteractiveUI::SetInteractive(const FString& Key,
                                    const FString& Text) const
{
	DisplayKey->SetText(FText::FromString(Key));
	DisplayText->SetText(FText::FromString(Text));
}
