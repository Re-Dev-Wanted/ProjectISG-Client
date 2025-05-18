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

public:
	void SetWidget(uint16 MaterialId, uint16 Count, const FString& Name, UTexture2D* Image);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> OwningCountText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequiredCountText;

private:
	uint16 Id = 0;
};
