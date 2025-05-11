#pragma once

#include "CoreMinimal.h"
#include "PlacementData.generated.h"

UENUM(BlueprintType)
enum ERotateDirection : uint8
{
	None = 0,
	North = 1 << 0,
	West = 1 << 1,
	South = 1 << 2,
	East = 1 << 3,

	All = North | West | East | South
};

// 북 -> 서 -> 남 -> 동
FORCEINLINE ERotateDirection operator <<(ERotateDirection Direction, uint8 Shift)
{
	if (Direction == None)
	{
		return Direction;
	}

	constexpr uint8 All = ERotateDirection::All;
	const uint8 Value = static_cast<uint8>(Direction);

	constexpr uint8 BitCount = 4;
	Shift %= BitCount;

	return static_cast<ERotateDirection>(((Value >> Shift) | (Value << (BitCount - Shift))) & All);
}

// 북 -> 동 -> 남 -> 서
FORCEINLINE ERotateDirection operator >>(ERotateDirection Direction, uint8 Shift)
{
	if (Direction == None)
	{
		return Direction;
	}

	constexpr uint8 All = ERotateDirection::All;
	const uint8 Value = static_cast<uint8>(Direction);

	constexpr uint8 BitCount = 4;
	Shift %= BitCount;

	return static_cast<ERotateDirection>(((Value << Shift) | (Value >> (BitCount - Shift))) & All);
}

FORCEINLINE float GetDegrees(ERotateDirection Direction)
{
	switch (Direction)
	{
	case West:
		return 90.f;
	case South:
		return 180.f;
	case East:
		return 270.f;
	default:
		return 0.f;
	}
}
