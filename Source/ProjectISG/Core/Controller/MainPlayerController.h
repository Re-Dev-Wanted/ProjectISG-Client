#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerController.generated.h"

enum class EUIName : uint32;

class UItemInfo;
class UUIManageComponent;
class UQuestManageComponent;

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
	void Alert(const EAlertType AlertType, const FString& Message
				, const float Time = 2.f);

	// 월드 퀘스트 실행
	UFUNCTION(BlueprintCallable)
	void StartQuest(const FString& QuestId);

	// 특정 플레이어에게만 특정 퀘스트를 진행 시킴
	UFUNCTION(BlueprintCallable)
	void StartQuestToPlayer(const FString& QuestId);

	UFUNCTION(BlueprintCallable)
	void EndQuest();

	UFUNCTION(Client, Reliable)
	void Client_StartQuestToPlayer(const FString& QuestId);

	UFUNCTION(BlueprintCallable)
	void StartScene(const FString SceneId) const;

	void ShowLoadingUIAndCreateSession(bool bIsServerTravel);

#pragma region GetUI
	TObjectPtr<UUIC_MainHUD> GetMainHUD() const;
	TObjectPtr<UUIC_InventoryUI> GetInventoryUI() const;
#pragma endregion

	GETTER(TObjectPtr<UUIManageComponent>, UIManageComponent)
	GETTER(TObjectPtr<UQuestManageComponent>, QuestManageComponent)

	UFUNCTION(Client, Reliable)
	void Client_ResetWidgetAndPushTimeAlert();

	UFUNCTION(Client, Reliable)
	void Client_EndQuestToPlayer();

#pragma region SetOwner
	UFUNCTION(Server, Reliable)
	void Server_SetOwnerActor(AActor* ResponseActor);
#pragma endregion

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UUIManageComponent> UIManageComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UQuestManageComponent> QuestManageComponent;

#pragma region Quest
	UFUNCTION(Server, Reliable)
	void Server_StartQuest(const FString& QuestId);
#pragma endregion
};
