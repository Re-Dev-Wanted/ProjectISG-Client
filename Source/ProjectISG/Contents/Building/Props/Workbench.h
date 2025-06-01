#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "Workbench.generated.h"

UCLASS()
class PROJECTISG_API AWorkbench : public APlacement, public IUIHandler
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual bool GetCanTouch() const override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual void OnClosed() override;

	UFUNCTION()
	void UnlockPlayer();

	UFUNCTION(Client, Reliable)
	void Client_UnlockPlayer();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetInteractingPlayerPosition(class AMainPlayerCharacter* Player);
};
