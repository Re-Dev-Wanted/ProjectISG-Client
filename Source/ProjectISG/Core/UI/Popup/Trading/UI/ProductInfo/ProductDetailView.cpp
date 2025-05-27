#include "ProductDetailView.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UProductDetailView::UpdateUI(FString Name, FString Desc,
                                  TSoftObjectPtr<UTexture2D> Image)
{
	SelectedItemName->SetText(FText::FromString(Name));
	ItemDesc->SetText(FText::FromString(Desc));

	UTexture2D* LoadedTexture = Image.LoadSynchronous();
	
	ItemIcon->SetBrushFromTexture(LoadedTexture);
}
