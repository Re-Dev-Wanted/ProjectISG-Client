#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_CookingQTEKeyPressWidget.generated.h"

enum class ECookingQTEKey : uint8;
struct FInputActionValue;
class UInputAction;

UCLASS()
class PROJECTISG_API UUIC_CookingQTEKeyPressWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	// TODO: 조만간 QTE 자체를 공통 컴포넌트로 빼도 괜찮을 것 같음
	void StartQTE();

	void CheckQTE(const uint8 CookingQTEKey);

	void DecreaseTime(const float DeltaTime);
};
