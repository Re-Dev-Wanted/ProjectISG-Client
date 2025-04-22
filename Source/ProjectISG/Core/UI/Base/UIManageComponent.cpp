#include "UIManageComponent.h"

#include "RootHUD.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UUIManageComponent::Initialize(URootHUD* NewRootHUD)
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	RootHUD = NewRootHUD;

	for (auto WidgetClass : WidgetClasses)
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

void UUIManageComponent::PushWidget(const EUIName Key)
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	if (!WidgetLayers.Find(Key))
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner());

	if (WidgetLayers[Key] != EUILayer::Gameplay)
	{
		PC->SetShowMouseCursor(true);
		PC->DisableInput(PC);
	}

	WidgetStack.Add(Key);
	UUserWidget* NewWidget = WidgetInstances.FindRef(Key);

	if (!NewWidget)
	{
		NewWidget = CreateWidget(PC, WidgetClasses.FindRef(Key));
		WidgetInstances.Add(Key, NewWidget);
		RootHUD->AddWidgetToLayer(NewWidget, WidgetLayers[Key]);
	}

	NewWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUIManageComponent::PopWidget()
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

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	WidgetStack.Pop();

	if (WidgetLayers[LastWidgetName] != EUILayer::Gameplay && WidgetLayers[
		WidgetStack.Last()] == EUILayer::Gameplay)
	{
		PC->SetShowMouseCursor(false);
		PC->EnableInput(PC);
	}
}

bool UUIManageComponent::IsPlayerInLocalControlled() const
{
	if (!GetOwner())
	{
		return false;
	}

	const APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC)
	{
		return false;
	}

	return PC->IsLocalController();
}

EUIName UUIManageComponent::GetLastStackUI() const
{
	return WidgetStack.Last();
}
