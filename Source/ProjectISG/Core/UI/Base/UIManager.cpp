#include "UIManager.h"

#include "RootHUD.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UUIManager::Initialize(APlayerController* PC, URootHUD* NewRootHUD)
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	RootHUD = NewRootHUD;
	OwnerController = PC;

	for (TTuple<EUIName, TSubclassOf<UUserWidget>> WidgetClass : WidgetClasses)
	{
		if (FEnumUtil::GetClassEnumKeyAsString(WidgetClass.Key).StartsWith(
			TEXT("Gameplay_")))
		{
			WidgetLayers.Add(WidgetClass.Key, EUILayer::Gameplay);
		}

		if (FEnumUtil::GetClassEnumKeyAsString(WidgetClass.Key).StartsWith(
			TEXT("Popup_")))
		{
			WidgetLayers.Add(WidgetClass.Key, EUILayer::Popup);
		}

		if (FEnumUtil::GetClassEnumKeyAsString(WidgetClass.Key).StartsWith(
			TEXT("Modal_")))
		{
			WidgetLayers.Add(WidgetClass.Key, EUILayer::Modal);
		}

		if (FEnumUtil::GetClassEnumKeyAsString(WidgetClass.Key).StartsWith(
			TEXT("Loading_")))
		{
			WidgetLayers.Add(WidgetClass.Key, EUILayer::Loading);
		}
	}
}

void UUIManager::PushWidget(const EUIName Key)
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	WidgetStack.Add(Key);
	UUserWidget* NewWidget = WidgetInstances.FindRef(Key);

	if (!NewWidget)
	{
		NewWidget = CreateWidget<UUserWidget>(
			OwnerController, WidgetClasses.FindRef(Key));
		WidgetInstances.Add(Key, NewWidget);
		RootHUD->AddWidgetToLayer(NewWidget, WidgetLayers[Key]);
	}

	NewWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUIManager::PopWidget()
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	if (WidgetStack.IsEmpty())
	{
		return;
	}

	const EUIName LastWidgetName = WidgetStack.Last();
	WidgetInstances[LastWidgetName]->SetVisibility(ESlateVisibility::Hidden);

	WidgetStack.Pop();
}

bool UUIManager::IsPlayerInLocalControlled() const
{
	if (!OwnerController)
	{
		return false;
	}

	return OwnerController->IsLocalController();
}
