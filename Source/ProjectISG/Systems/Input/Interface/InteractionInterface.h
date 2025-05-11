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
	virtual FString GetInteractiveDisplayText() const { return TEXT(""); }
	virtual FString GetInteractiveDisplayKey() const { return TEXT(""); }
	virtual FString GetTouchDisplayText(AActor* Causer) const { return TEXT(""); }
	virtual FString GetTouchDisplayKey() const { return TEXT(""); }

	virtual void OnInteractive(AActor* Causer);

	virtual void OnTouch(AActor* Causer);

	virtual void OnInteractiveResponse(AActor* Causer);

	virtual void OnTouchResponse(AActor* Causer);
};
