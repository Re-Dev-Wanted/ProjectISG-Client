#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Core/UI/UIEnum.h"
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
	void PushUI(const EUIName UIName);
	void PopUI();

#pragma region GetUI
	TObjectPtr<UUIC_MainHUD> GetMainHUD() const;
	TObjectPtr<UUIC_InventoryUI> GetInventoryUI() const;
#pragma endregion

	GETTER(TObjectPtr<UUIManageComponent>, UIManageComponent)
	// TODO: UI Model로 이전하는 것을 권장함.
	GETTER_SETTER(uint32, ClickedProductId)
	// TODO: UI Model로 이전하는 것을 권장함.
	GETTER_SETTER(uint32, ClickedInventoryItem)

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;

	// TODO: UI Model로 이전하는 것을 권장함.
	UPROPERTY(EditAnywhere)
	uint32 ClickedProductId;

	// TODO: UI Model로 이전하는 것을 권장함.
	UPROPERTY(EditAnywhere)
	uint32 ClickedInventoryItem;
};
