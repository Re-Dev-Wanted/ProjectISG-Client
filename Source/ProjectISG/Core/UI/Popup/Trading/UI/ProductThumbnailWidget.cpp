#include "ProductThumbnailWidget.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void UProductThumbnailWidget::SetWidget(TSoftObjectPtr<UTexture2D> Image)
{
	UTexture2D* LoadedImage = Image.LoadSynchronous();
	Thumbnail->SetBrushFromTexture(LoadedImage);
}

void UProductThumbnailWidget::SetCount(uint32 Count)
{
	Bottom->SetVisibility(Count > 0? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	CountText->SetText(FText::FromString(FString::FromInt(Count)));
}
