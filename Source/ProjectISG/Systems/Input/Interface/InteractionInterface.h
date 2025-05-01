#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class AMainPlayerController;

UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTISG_API IInteractionInterface
{
	GENERATED_BODY()

public:
	virtual bool GetCanInteractive() const { return false; }
	virtual bool GetCanTouch() const { return false; }
	virtual FString GetDisplayText() const { return TEXT(""); }
	virtual FString GetDisplayKey() const { return TEXT(""); }

	virtual void OnInteractive(AActor* Causer);

	virtual void OnTouch(AActor* Causer);

	virtual void OnInteractiveResponse();
};
