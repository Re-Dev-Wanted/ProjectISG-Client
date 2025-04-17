#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UMainHUD;
class UInventoryUI;

UCLASS()
class PROJECTISG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void InitializeHUD();
	void ToggleInventoryUI(const bool IsShow);

protected:
	virtual void BeginPlay() override;

private:
#pragma region UI
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
