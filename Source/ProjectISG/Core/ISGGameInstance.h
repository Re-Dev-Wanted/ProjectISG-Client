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
	void CreateGameSessionIdAndCreateSession();

	void CreateSession();

	void JoinFoundSession();
#pragma endregion 

#pragma region GETTERSETTER
	GETTER_SETTER(FString, SessionId)

	GETTER_SETTER(bool, SessionCreateSuccess)

	GETTER_SETTER(bool, IsServerTravel)
#pragma endregion
	
protected:
	virtual void Init() override;

	UFUNCTION()
	void OnCompleteCreate(FName SessionName, bool IsCreateSuccess);
	
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type);

private:
	UPROPERTY()
	FString SessionId;

	bool SessionCreateSuccess = false;

	bool IsServerTravel;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
};
