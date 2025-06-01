#include "InventoryNoticeWidgetItem.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventoryNoticeWidgetItem::OnNotify(TSoftObjectPtr<UTexture2D> Image, FString Name)
{
	ItemThumbnail->SetBrushFromSoftTexture(Image);
	ItemName->SetText(FText::FromString(Name));

	if (!AppearAnimation)
	{
		return;
	}
	
	UUMGSequencePlayer* AnimationPlayer = PlayAnimation(AppearAnimation);

	AnimationPlayer->OnSequenceFinishedPlaying().AddUObject(this, &UInventoryNoticeWidgetItem::AppearAnimationFinished);
}

void UInventoryNoticeWidgetItem::NativeConstruct()
{
	Super::NativeConstruct();

	// Border->SetRenderTranslation(FVector2D(500.f, 0));
}

void UInventoryNoticeWidgetItem::AppearAnimationFinished(UUMGSequencePlayer& AnimationPlayer)
{
	GetWorld()->GetTimerManager()
	.SetTimerForNextTick(
		[this]()
	   {
		   RemoveFromParent();
	   }
	);
}


