#pragma once

#include "CookingEnum.generated.h"

UENUM()
enum class EKitchenFurnitureCinematicStatus : uint8
{
	None, StartCooking, EndCooking,
};

UENUM()
enum class ECookingTool : uint8
{
	None, FryingPan, Wok
};

UENUM()
enum class ECookingQTEType: uint8
{
	None, KeyPressArray, KeyChargeGage
};

UENUM()
enum class ECookingQTEKey: uint8
{
	Q, W, E, A, S, D, Space
};

UENUM()
enum class ECookingQTEStatus: uint8
{
	None, Trying, Success, Fail
};
