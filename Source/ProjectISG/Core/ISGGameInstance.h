#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "ISGGameInstance.generated.h"

UCLASS()
class PROJECTISG_API UISGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UISGGameInstance();

#pragma region SessionFunc
	void CreateGameSessionId();

	void CreateSession();

	void JoinFoundSession();

	void DestroySessionAndMoveLevel(TSoftObjectPtr<UWorld> Level);
#pragma endregion

#pragma region GETTERSETTER
	GETTER_SETTER(FString, SessionId)

	GETTER_SETTER(bool, IsServerTravel)
#pragma endregion

protected:
	virtual void Init() override;

	UFUNCTION()
	void OnCompleteCreate(FName SessionName, bool IsCreateSuccess);

	void OnJoinSession(FName SessionName,
	                   EOnJoinSessionCompleteResult::Type Type);

private:
	UPROPERTY(EditDefaultsOnly)
	FString SessionId;

	UPROPERTY(EditDefaultsOnly)
	bool IsServerTravel = false;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
