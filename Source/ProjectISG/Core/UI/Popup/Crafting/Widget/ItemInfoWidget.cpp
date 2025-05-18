#include "ItemInfoWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Id = 0;
	ItemName->SetText(FText::GetEmpty());
	Thumbnail->SetBrushFromTexture(nullptr);
	SelectButton->OnClicked.AddDynamic(this, &ThisClass::OnExecuteClickEvent);
}

void UItemInfoWidget::SetWidget(uint16 DataId, const FString& Name, UTexture2D* Image, FOnSelectedCraftingRecipe OnSelectedRecipe)
{
	Id = DataId;
	ItemName->SetText(FText::FromString(Name));
	Thumbnail->SetBrushFromTexture(Image);

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

void UItemInfoWidget::OnExecuteClickEvent()
{
	if (!OnClickedSelectedButton.ExecuteIfBound(Id))
	{
		
	}
}

