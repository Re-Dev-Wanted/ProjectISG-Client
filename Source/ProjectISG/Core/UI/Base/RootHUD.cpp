#include "RootHUD.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
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

	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(NewWidget->Slot);
	if (CanvasPanelSlot)
	{
		CanvasPanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
		CanvasPanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		CanvasPanelSlot->SetPosition(FVector2D(0.f, 0.f));
	}
}
