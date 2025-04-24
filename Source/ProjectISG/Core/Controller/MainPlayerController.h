#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerController.generated.h"

enum class EUIName : uint32;

class UItemInfo;
class UUIManageComponent;

class UUIC_MainHUD;
class UUIC_InventoryUI;

UCLASS()
class PROJECTISG_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainPlayerController();

	void OpenInventory();
	void ShowItemInfo(const uint16 InventoryIndex) const;
	void RemoveItemInfo() const;
	void PopUI();


	TObjectPtr<UUIC_MainHUD> GetMainHUD() const;

	TObjectPtr<UUIC_InventoryUI> GetInventoryUI() const;

	GETTER(TObjectPtr<UUIManageComponent>, UIManageComponent)

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;
};
