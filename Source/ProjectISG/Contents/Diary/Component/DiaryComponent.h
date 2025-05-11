#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "Interfaces/IHttpRequest.h"
#include "ProjectISG/Utils/MacroUtil.h"

#include "DiaryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UDiaryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDiaryComponent();

	UFUNCTION()
	void GenerateDiary();

	UFUNCTION(Client, Reliable)
	void Client_GenerateDiary();

	UFUNCTION(Server, Reliable)
	void Server_ChangeWriteDiary(bool value, class AMainPlayerCharacter* Player);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ChangeWriteDiary(bool value, class AMainPlayerCharacter* Player);
	
	GETTER_SETTER(bool, bWriteDiary);

protected:
	virtual void BeginPlay() override;

private:
	FApiResponse GenerateDiaryResponse = {};

	UPROPERTY(EditAnywhere)
	bool bWriteDiary = false;
};
