#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RootHUD.generated.h"

enum class EUILayer: uint8;
class UCanvasPanel;

UCLASS()
class PROJECTISG_API URootHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddWidgetToLayer(UUserWidget* NewWidget, const EUILayer NewLayer);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GameplayLayer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> PopupLayer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ModalLayer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LoadingLayer;
};
