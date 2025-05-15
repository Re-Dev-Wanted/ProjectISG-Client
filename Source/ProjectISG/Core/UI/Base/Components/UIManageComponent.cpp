#include "UIManageComponent.h"

#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "ProjectISG/Utils/EnumUtil.h"

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
			WidgetLayers.Add(Elem.Key, EUILayer::Gameplay);
		}
		else if (NameStr.StartsWith(TEXT("Popup_")))
		{
			WidgetLayers.Add(Elem.Key, EUILayer::Popup);
		}
		else if (NameStr.StartsWith(TEXT("Modal_")))
		{
			WidgetLayers.Add(Elem.Key, EUILayer::Modal);
		}
		else if (NameStr.StartsWith(TEXT("Loading_")))
		{
			WidgetLayers.Add(Elem.Key, EUILayer::Loading);
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

	if (!WidgetStack.IsEmpty())
	{
		const EUIName LastKey = WidgetStack.Last();

		if (WidgetLayers[Key] != EUILayer::Modal && ControllerInstances.
			Contains(LastKey))
		{
			ControllerInstances[LastKey]->EndShowUI();
		}
	}

	// 동일한 Key가 들어가 있으면 순서를 앞으로 이동시킨다
	// UI는 반드시 Key 하나당 하나의 UI로 바인딩 된다는 비즈니스
	// 규칙을 명시한다. [25.04.24]
	// if (WidgetStack.Find(Key) != INDEX_NONE)
	// {
	// 	WidgetStack.Remove(Key);
	// }

	WidgetStack.Add(Key);

	if (!ControllerInstances.Contains(Key))
	{
		UBaseUIView* NewView = CreateWidget<UBaseUIView>(PC, ViewClasses[Key]);
		NewView->AddToViewport(static_cast<int32>(WidgetLayers[Key]));
		ControllerInstances.Add(Key, NewView->GetController());
	}

	ControllerInstances[Key]->StartShowUI(WidgetLayers[Key]);

	PrintAllWidgetStackToDebug();
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
		ControllerInstances[LastKey]->EndShowUI();
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
		ControllerInstances[PrevKey]->StartShowUI(WidgetLayers[PrevKey]);
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (WidgetLayers[LastKey] != EUILayer::Gameplay && WidgetLayers[PrevKey] ==
		EUILayer::Gameplay)
	{
		PC->SetShowMouseCursor(false);
	}

	PrintAllWidgetStackToDebug();
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

TSubclassOf<UBaseUIView> UUIManageComponent::GetViewClasses(EUIName Key)
{
	return ViewClasses[Key];
}

bool UUIManageComponent::HasViewUI(const EUIName Key)
{
	return WidgetStack.Find(Key) != INDEX_NONE;
}

void UUIManageComponent::PrintAllWidgetStackToDebug()
{
	FString StackText;
	for (int i = 0; i < WidgetStack.Num(); i++)
	{
		StackText += FEnumUtil::GetClassEnumKeyAsString(WidgetStack[i]);

		if (i != WidgetStack.Num() - 1)
		{
			StackText += TEXT(",");
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Current Widget Stack: [%s]"), *StackText);
}
