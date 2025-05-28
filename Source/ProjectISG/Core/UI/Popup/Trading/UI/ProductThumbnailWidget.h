#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProductThumbnailWidget.generated.h"

class UOverlay;
class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class PROJECTISG_API UProductThumbnailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidget(TSoftObjectPtr<UTexture2D> Image);

	void SetCount(uint32 Count);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> Bottom;
};
