#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManageComponent.generated.h"

class URootHUD;
enum class EUILayer : uint8;
enum class EUIName : uint32;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UUIManageComponent : public UActorComponent
{
public:
	GENERATED_BODY()

	void Initialize();

	void PushWidget(const EUIName Key);
	void PopWidget();
	EUIName GetLastStackUI() const;

	UPROPERTY()
	TMap<EUIName, TObjectPtr<UUserWidget>> WidgetInstances;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TMap<EUIName, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	TMap<EUIName, EUILayer> WidgetLayers;

	UPROPERTY()
	TArray<EUIName> WidgetStack;

	bool IsPlayerInLocalControlled() const;
};
