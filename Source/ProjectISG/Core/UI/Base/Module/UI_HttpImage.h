#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "UI_HttpImage.generated.h"

class UImage;

UCLASS()
class PROJECTISG_API UUI_HttpImage : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetImagePath(const FString& ImageUrl);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image;

	TObjectPtr<UTexture2D> LoadTextureFromImageData(
		const TArray<uint8>& ImageData);

	FApiResponse ImageApiResponse;
};
