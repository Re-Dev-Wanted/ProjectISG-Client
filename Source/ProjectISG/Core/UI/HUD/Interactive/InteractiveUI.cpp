#include "InteractiveUI.h"

#include "InteractiveItemUI.h"
#include "Components/VerticalBox.h"

void UInteractiveUI::NativeConstruct()
{
	Super::NativeConstruct();

	//TODO:: 위젯 따로 만들어서 세팅해야한다.
	ExecuteInteractUI->SetInteractive(TEXT("RM"), TEXT("설치하기"));
	RightRotateUI->SetInteractive(TEXT("E"), TEXT("회전하기"));
}

void UInteractiveUI::SetInteractive(bool Visible)
{
	VerticalView->SetVisibility(Visible? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UInteractiveUI::SetInteractive(const FString& Key,
                                    const FString& Text)
{
	ClearItems();
	
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
	const FString& Text)
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

void UInteractiveUI::SetPlacementIndicator(bool Visible, bool Rotate)
{
	VerticalView_Placement->SetVisibility(Visible? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	Vertical_Root->Modify();
}

void UInteractiveUI::ClearItems()
{
	for (UWidget* Item : VerticalView->GetAllChildren())
	{
		Item->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UInteractiveUI::IsInteractiveHidden()
{
	return VerticalView->GetVisibility() == ESlateVisibility::Collapsed;
}
