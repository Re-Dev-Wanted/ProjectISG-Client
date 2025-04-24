#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
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
	GETTER(bool, CanInteractive)
	GETTER(bool, CanTouch)
	GETTER(FString, DisplayText)
	GETTER(FString, DisplayKey)

	virtual void OnInteractive(AActor* Causer);

	virtual void OnTouch(AActor* Causer);

protected:
	FString DisplayText;
	FString DisplayKey;

	// F키 같은 별도의 Interactive 키에 대한 대응
	bool CanInteractive = false;
	// 마우스 우클릭에 대한 대응 처리
	bool CanTouch = false;
};
