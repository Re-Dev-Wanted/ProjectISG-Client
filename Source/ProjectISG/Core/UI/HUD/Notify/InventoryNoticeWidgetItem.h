#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryNoticeWidgetItem.generated.h"

class UBorder;
class UTextBlock;
class UImage;
class UWidgetAnimation;
class UTexture2D;

UCLASS()
class PROJECTISG_API UInventoryNoticeWidgetItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void OnNotify(TSoftObjectPtr<UTexture2D> Image, FString Name);

protected:
	virtual void NativeConstruct() override;

	void AppearAnimationFinished(UUMGSequencePlayer& AnimationPlayer);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> AppearAnimation;
	
};
