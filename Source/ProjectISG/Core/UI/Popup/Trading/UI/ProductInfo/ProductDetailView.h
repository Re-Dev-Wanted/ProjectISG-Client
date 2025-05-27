#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProductDetailView.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTISG_API UProductDetailView : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateUI(FString Name, FString Desc, TSoftObjectPtr<UTexture2D> Image);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProductPrice;
};
