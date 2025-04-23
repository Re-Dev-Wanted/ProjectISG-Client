// UIManageComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "UIManageComponent.generated.h"

class UBaseUIView;
class UBaseUIController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UUIManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void Initialize();

	void PushWidget(const EUIName Key);
	void PopWidget();
	EUIName GetLastStackUI() const;

	UPROPERTY()
	TMap<EUIName, TObjectPtr<UBaseUIController>> ControllerInstances;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta=(AllowPrivateAccess = true))
	TMap<EUIName, TSubclassOf<UBaseUIView>> ViewClasses;

	UPROPERTY()
	TMap<EUIName, EUILayer> WidgetLayers;

	UPROPERTY()
	TArray<EUIName> WidgetStack;

	bool IsPlayerInLocalControlled() const;
};
