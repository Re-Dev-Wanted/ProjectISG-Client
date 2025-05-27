#include "TradingTabButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTradingTabButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	FButtonStyle Style;
	
	FSlateBrush SlateBrush;

	SlateBrush.SetImageSize(FDeprecateSlateVector2D(32, 80));
	SlateBrush.TintColor = ActiveColor;
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

	ButtonText->SetColorAndOpacity(ActiveTextColor);
	ButtonText->SetText(FText::FromString(Text));
}
