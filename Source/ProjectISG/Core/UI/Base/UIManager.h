#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UIManager.generated.h"

class URootHUD;
enum class EUILayer : uint8;
enum class EUIName : uint32;

UCLASS()
class PROJECTISG_API UUIManager : public UObject
{
public:
	GENERATED_BODY()

	void Initialize(APlayerController* PC, URootHUD* NewRootHUD);

	void PushWidget(const EUIName Key);
	void PopWidget();

private:
	UPROPERTY()
	TMap<EUIName, TObjectPtr<UUserWidget>> WidgetInstances;

	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TMap<EUIName, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	TMap<EUIName, EUILayer> WidgetLayers;

	UPROPERTY()
	TArray<EUIName> WidgetStack;

	UPROPERTY()
	TObjectPtr<URootHUD> RootHUD;

	UPROPERTY()
	TObjectPtr<APlayerController> OwnerController;

	bool IsPlayerInLocalControlled() const;
};
