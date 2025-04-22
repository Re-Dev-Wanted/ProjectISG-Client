#include "RootHUD.h"

#include "Components/CanvasPanel.h"
#include "ProjectISG/Core/UI/UIEnum.h"

void URootHUD::AddWidgetToLayer(UUserWidget* NewWidget, const EUILayer NewLayer)
{
	switch (NewLayer)
	{
	case EUILayer::Gameplay:
		{
			GameplayLayer->AddChild(NewWidget);
		}
	case EUILayer::Popup:
		{
			PopupLayer->AddChild(NewWidget);
		}
	case EUILayer::Modal:
		{
			ModalLayer->AddChild(NewWidget);
		}
	case EUILayer::Loading:
		{
			LoadingLayer->AddChild(NewWidget);
		}
	}
}
