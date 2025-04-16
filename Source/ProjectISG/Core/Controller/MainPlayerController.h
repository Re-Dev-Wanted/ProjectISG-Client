#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UInventoryUI;

UCLASS()
class PROJECTISG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI",
		meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventoryUI> InventoryUIClass;

	UPROPERTY()
	TObjectPtr<UInventoryUI> InventoryUI;
};
