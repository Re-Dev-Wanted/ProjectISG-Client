
#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "Altar.generated.h"

UCLASS()
class PROJECTISG_API AAltar : public APlacement
{
	GENERATED_BODY()

public:
	AAltar();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractive(AActor* Causer) override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual bool GetCanInteractive() const override;

	UFUNCTION()
	void MoveToLobby();
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	uint32 OfferingFoodId;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	FString EndingScene;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true),
		Category = Setting)
	TSoftObjectPtr<UWorld> LobbyLevel;

};