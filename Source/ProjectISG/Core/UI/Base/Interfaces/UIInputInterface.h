#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIInputInterface.generated.h"

class AMainPlayerController;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UINTERFACE()
class UUIInputInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTISG_API IUIInputInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeUIInput(APlayerController* BasePC);

	void EscapeUI();

protected:
	virtual void InitializeInputData(UEnhancedInputLocalPlayerSubsystem
	                                 *
	                                 EnhancedInputLocalPlayerSubsystem,
	                                 UEnhancedInputComponent*
	                                 EnhancedInputComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultUIMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BaseEscapeAction;

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
};
