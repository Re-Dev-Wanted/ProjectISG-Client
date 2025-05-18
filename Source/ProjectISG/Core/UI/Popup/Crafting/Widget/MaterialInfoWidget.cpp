#include "MaterialInfoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UMaterialInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Id = 0;
	Thumbnail->SetBrushFromMaterial(nullptr);
	
	RequiredCountText->SetText(FText::GetEmpty());
	OwningCountText->SetText(FText::GetEmpty());
}

void UMaterialInfoWidget::SetWidget(uint16 MaterialId, uint16 OwningCount, uint16 RequiredCount, const FString& Name, UTexture2D* Image)
{
	Id = MaterialId;

	Thumbnail->SetBrushFromTexture(Image);
	
	OwningCountText->SetText(FText::FromString(FString::FromInt(OwningCount)));

	FColor Color = OwningCount < RequiredCount? FColor::Red : FColor::White;
	
	OwningCountText->SetColorAndOpacity(FSlateColor(Color));
	
	RequiredCountText->SetText(FText::FromString(FString::FromInt(RequiredCount)));
}

