#include "TradingTabButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTradingTabButton::OnActive(bool IsActive)
{
	FSlateColor ButtonColor = IsActive? ActiveColor : DeactiveColor;
	FSlateColor TextColor = IsActive? ActiveTextColor : DeactiveTextColor;

	SetStyleInternal(ButtonColor, TextColor);
}

void UTradingTabButton::SetStyleInternal(FSlateColor ButtonColor, FSlateColor
 TextColor)
{
	FButtonStyle Style;
	
	FSlateBrush SlateBrush;

	SlateBrush.SetImageSize(FDeprecateSlateVector2D(32, 80));
	SlateBrush.TintColor = ButtonColor;
	SlateBrush.DrawAs = ESlateBrushDrawType::RoundedBox;

	FSlateBrushOutlineSettings OutlineSettings;

	OutlineSettings.CornerRadii = Corner;
	OutlineSettings.Width = 0;
	OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	OutlineSettings.bUseBrushTransparency = true;
	
	SlateBrush.OutlineSettings = OutlineSettings;
	
	Style.SetNormal(SlateBrush);
	Style.SetHovered(SlateBrush);
	Style.SetPressed(SlateBrush);
	Style.SetDisabled(SlateBrush);
	
	Button->SetStyle(Style);

	ButtonText->SetColorAndOpacity(TextColor);
}

void UTradingTabButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	ButtonText->SetText(FText::FromString(Text));
	SetStyleInternal(ActiveColor, ActiveTextColor);
}
