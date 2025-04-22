#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerController.generated.h"

class UUIManager;
class URootHUD;
class UItemInfo;
class UMainHUD;
class UInventoryUI;

UCLASS()
class PROJECTISG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void InitializeHUD();
	void ToggleInventoryUI(const bool IsShow);
	void ShowItemInfo(const uint16 InventoryIndex) const;
	void RemoveItemInfo() const;

	GETTER(TObjectPtr<UMainHUD>, MainHUD)
	GETTER(TObjectPtr<UInventoryUI>, InventoryUI)

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
#pragma region UI
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<URootHUD> RootHUDClass;

	UPROPERTY()
	TObjectPtr<UUIManager> UIManager;

	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainHUD> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UMainHUD> MainHUD;

	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventoryUI> InventoryUIClass;

	UPROPERTY()
	TObjectPtr<UInventoryUI> InventoryUI;
#pragma endregion
};
