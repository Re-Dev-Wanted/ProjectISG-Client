#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerController.generated.h"

enum class EUIName : uint32;
class UUIManageComponent;
class URootHUD;
class UItemInfo;
class UMainHUD;
class UInventoryUI;

UCLASS()
class PROJECTISG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	void ToggleInventoryUI(const bool IsShow);
	void ShowItemInfo(const uint16 InventoryIndex) const;
	void RemoveItemInfo() const;
	void PopUI();

	TObjectPtr<UMainHUD> GetMainHUD() const;
	TObjectPtr<UInventoryUI> GetInventoryUI() const;
	GETTER(TObjectPtr<UUIManageComponent>, UIManageComponent)

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;

#pragma region UI
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<URootHUD> RootHUDClass;
#pragma endregion
};
