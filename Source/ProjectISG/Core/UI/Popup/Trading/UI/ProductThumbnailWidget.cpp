#include "ProductThumbnailWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UProductThumbnailWidget::SetWidget(TSoftObjectPtr<UTexture2D> Image,
                                        uint32 Count)
{
	UTexture2D* LoadedImage = Image.LoadSynchronous();
	Thumbnail->SetBrushFromTexture(LoadedImage);

	CountText->SetText(FText::FromString(FString::FromInt(Count)));
}
