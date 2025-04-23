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
			ControllerInstances[LastKey]->GetView()->SetVisibility(
				ESlateVisibility::Hidden);
		}
	}

	WidgetStack.Add(Key);

	if (!ControllerInstances.Contains(Key))
	{
		UBaseUIView* NewView = CreateWidget<UBaseUIView>(PC, ViewClasses[Key]);
		NewView->AddToViewport(static_cast<int32>(WidgetLayers[Key]));
		ControllerInstances.Add(Key, NewView->GetController());
	}

	ControllerInstances[Key]->AppearUI();
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
		ControllerInstances[LastKey]->DisappearUI();
	}

	WidgetStack.Pop();

	// 여기서부터는 제거된 Stack을 기반으로 동작함
	if (WidgetStack.IsEmpty())
	{
		return;
	}

	// 제거된 이후 남은 Key 기반으로 UI 노출 처리
	const EUIName PrevKey = WidgetStack.Last();
	if (ControllerInstances.Contains(PrevKey))
	{
		ControllerInstances[PrevKey]->AppearUI();
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

bool UUIManageComponent::HasViewUI(const EUIName Key)
{
	return WidgetStack.Find(Key) != INDEX_NONE;
}
