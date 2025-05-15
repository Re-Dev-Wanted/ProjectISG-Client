#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerController.generated.h"

class UQuestManageComponent;
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

	UFUNCTION(BlueprintCallable)
	void PushUI(const EUIName UIName);
	UFUNCTION(BlueprintCallable)
	void PopUI();

	UFUNCTION(BlueprintCallable)
	void StartQuest(const FString& QuestId) const;

	UFUNCTION(BlueprintCallable)
	void StartScene(const FString& SceneId) const;

#pragma region GetUI
	TObjectPtr<UUIC_MainHUD> GetMainHUD() const;
	TObjectPtr<UUIC_InventoryUI> GetInventoryUI() const;
#pragma endregion

	GETTER(TObjectPtr<UUIManageComponent>, UIManageComponent)
	GETTER(TObjectPtr<UQuestManageComponent>, QuestManageComponent)

	UFUNCTION(Client, Reliable)
	void Client_ResetWidgetAndPushTimeAlert();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UQuestManageComponent> QuestManageComponent;
};
