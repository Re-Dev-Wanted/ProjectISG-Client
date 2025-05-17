#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MaterialInfoWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UMaterialInfoWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OwningCountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredCountText;
};
