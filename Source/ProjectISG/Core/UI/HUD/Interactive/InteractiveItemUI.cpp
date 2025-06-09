
#include "InteractiveItemUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInteractiveItemUI::SetInteractive(const FString& Key,
	const FString& Text) const
{
	if (IconMap.Contains(Key))
	{
		KeyIcon->SetBrushFromSoftTexture(IconMap[Key]);
	}
	DisplayText->SetText(FText::FromString(Text));
}
