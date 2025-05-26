#include "ItemInfoWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SelectButton->OnClicked.IsBound())
	{
		return;
	}
	
	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnExecuteClickEvent);
}

void UItemInfoWidget::SetWidget(uint16 DataId, const FString& Name, FOnSelectedCraftingRecipe OnSelectedRecipe)
{
	Id = DataId;

	ItemName->SetText(FText::FromString(Name));

	if (OnClickedSelectedButton.IsBound())
	{
		OnClickedSelectedButton.Unbind();
	}

	OnClickedSelectedButton.BindLambda([OnSelectedRecipe](uint16 RecipeId)
	{
	   if (OnSelectedRecipe.IsBound())
	   {
		   OnSelectedRecipe.Execute(RecipeId);
	   }
	});
}

void UItemInfoWidget::SetImage(UTexture2D* Image)
{
	Thumbnail->SetBrushFromTexture(Image);
}

void UItemInfoWidget::OnExecuteClickEvent()
{
	if (OnClickedSelectedButton.ExecuteIfBound(Id))
	{
		// UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%d"), Id));
	}
}

