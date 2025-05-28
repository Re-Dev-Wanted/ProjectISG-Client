#include "ProductDetailView.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UProductDetailView::UpdateUI(FString Name, FText Desc, uint32 Price,
                                  TSoftObjectPtr<UTexture2D> Image)
{
	if (!IsVisible())
	{
		SetVisibility(ESlateVisibility::Visible);
	}

	SelectedItemName->SetText(FText::FromString(Name));
	ItemDesc->SetText(Desc);

	UTexture2D* LoadedTexture = Image.LoadSynchronous();

	ItemIcon->SetBrushFromTexture(LoadedTexture);
	ProductPrice->SetText(FText::FromString(FString::FromInt(Price)));
}

void UProductDetailView::OnHide()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UProductDetailView::NativeConstruct()
{
	Super::NativeConstruct();

	OnHide();
}
