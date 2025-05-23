#include "MaterialInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMaterialInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Id = 0;
	// Thumbnail->SetBrushFromMaterial(nullptr);
	//
	// RequiredCountText->SetText(FText::GetEmpty());
	// OwningCountText->SetText(FText::GetEmpty());
}

void UMaterialInfoWidget::SetWidget(uint16 MaterialId, uint16 OwningCount, uint16 RequiredCount, const FString& Name)
{
	Id = MaterialId;
	
	OwningCountText->SetText(FText::FromString(FString::FromInt(OwningCount)));

	FColor Color = OwningCount < RequiredCount? FColor::Red : FColor::White;
	
	OwningCountText->SetColorAndOpacity(FSlateColor(Color));
	
	RequiredCountText->SetText(FText::FromString(FString::FromInt(RequiredCount)));
}

void UMaterialInfoWidget::SetImage(UTexture2D* Image)
{
	Thumbnail->SetBrushFromTexture(Image);
}

