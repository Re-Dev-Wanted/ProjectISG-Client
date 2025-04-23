#include "UIManageComponent.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"

void UUIManageComponent::Initialize()
{
	if (!IsPlayerInLocalControlled())
	{
		return;
	}

	for (auto& Elem : ViewClasses)
	{
		const FString NameStr = StaticEnum<EUIName>()->GetNameStringByValue(
			static_cast<int64>(Elem.Key));
		if (NameStr.StartsWith(TEXT("Gameplay_")))
		{
			WidgetLayers.Add(
				Elem.Key, EUILayer::Gameplay);
		}
		else if (NameStr.StartsWith(TEXT("Popup_")))
		{
			WidgetLayers.Add(
				Elem.Key, EUILayer::Popup);
		}
		else if (NameStr.StartsWith(TEXT("Modal_")))
		{
			WidgetLayers.Add(
				Elem.Key, EUILayer::Modal);
		}
		else if (NameStr.StartsWith(TEXT("Loading_")))
		{
			WidgetLayers.Add(
				Elem.Key, EUILayer::Loading);
		}
	}
}

void UUIManageComponent::PushWidget(const EUIName Key)
{
	if (!IsPlayerInLocalControlled() || !WidgetLayers.Contains(Key))
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (WidgetLayers[Key] != EUILayer::Gameplay)
	{
		PC->SetShowMouseCursor(true);
		PC->DisableInput(PC);
	}

	if (!WidgetStack.IsEmpty())
	{
		const EUIName LastKey = WidgetStack.Last();
		if (ControllerInstances.Contains(LastKey))
		{
			// ControllerInstances[LastKey]->GetView()->SetVisibility(
			// 	ESlateVisibility::Hidden);
		}
	}

	WidgetStack.Add(Key);

	if (!ControllerInstances.Contains(Key))
	{
		UBaseUIView* NewView = CreateWidget<UBaseUIView>(PC, ViewClasses[Key]);
		NewView->AddToViewport(static_cast<int32>(WidgetLayers[Key]));
		ControllerInstances.Add(Key, NewView->GetController());
	}

	UBaseUIView* CurrentView = ControllerInstances[Key]->GetView();
	if (CurrentView)
	{
		CurrentView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUIManageComponent::PopWidget()
{
	if (!IsPlayerInLocalControlled() || WidgetStack.IsEmpty())
	{
		return;
	}

	const EUIName LastKey = WidgetStack.Last();
	if (ControllerInstances.Contains(LastKey))
	{
		ControllerInstances[LastKey]->GetView()->SetVisibility(
			ESlateVisibility::Hidden);
	}

	WidgetStack.Pop();

	if (WidgetStack.IsEmpty())
	{
		return;
	}

	const EUIName PrevKey = WidgetStack.Last();
	if (ControllerInstances.Contains(PrevKey))
	{
		ControllerInstances[PrevKey]->GetView()->SetVisibility(
			ESlateVisibility::SelfHitTestInvisible);
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (WidgetLayers[LastKey] != EUILayer::Gameplay && WidgetLayers[PrevKey] ==
		EUILayer::Gameplay)
	{
		PC->SetShowMouseCursor(false);
		PC->EnableInput(PC);
	}
}

bool UUIManageComponent::IsPlayerInLocalControlled() const
{
	const APlayerController* PC = Cast<APlayerController>(GetOwner());
	return PC && PC->IsLocalController();
}

EUIName UUIManageComponent::GetLastStackUI() const
{
	return WidgetStack.Last();
}
