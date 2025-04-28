#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingQTEUI.generated.h"

struct FInputActionValue;
class UInputAction;
enum class ECookingQTEType : uint8;
enum class ECookingQTEStatus : uint8;

UCLASS()
class PROJECTISG_API UUIC_CookingQTEUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void StartQTE(const ECookingQTEType QTEType);
	void SetQTEStatus(const ECookingQTEStatus QTEStatus);
	bool IsPlayerQTETrying();

protected:
	virtual void
	BindInputAction(UEnhancedInputComponent* InputComponent) override;

private:
	void InitializeDataToQTE();

	UFUNCTION()
	void OnPressQTEAction(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> QTEInputAction;
};
