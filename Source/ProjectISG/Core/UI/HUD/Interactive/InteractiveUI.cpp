#include "InteractiveUI.h"

#include "InteractiveItemUI.h"
#include "Components/VerticalBox.h"

void UInteractiveUI::SetInteractive(const FString& Key,
                                    const FString& Text) const
{
	int32 CurrentCount =  VerticalView->GetChildrenCount();

	if (CurrentCount > 0)
	{
		UInteractiveItemUI* ItemUI = Cast<UInteractiveItemUI>(VerticalView->GetChildAt(0));

		ItemUI->SetVisibility(ESlateVisibility::Visible);
		ItemUI->SetInteractive(Key, Text);

		return;
	}

	AddInteractive(Key, Text);
}

void UInteractiveUI::AddInteractive(const FString& Key,
	const FString& Text) const
{
	int32 CurrentCount =  VerticalView->GetChildrenCount();

	if (CurrentCount > 1)
	{
		UInteractiveItemUI* ItemUI = Cast<UInteractiveItemUI>(VerticalView->GetChildAt(1));

		ItemUI->SetVisibility(ESlateVisibility::Visible);
		ItemUI->SetInteractive(Key, Text);

		return;
	}
	
	UInteractiveItemUI* NewItemUI = CreateWidget<UInteractiveItemUI>(GetOwningPlayer(), 
	ItemFactory);

	NewItemUI->SetInteractive(Key, Text);

	VerticalView->AddChildToVerticalBox(NewItemUI);
}

void UInteractiveUI::ClearItems()
{
	for (UWidget* Item : VerticalView->GetAllChildren())
	{
		Item->SetVisibility(ESlateVisibility::Hidden);
	}
}
