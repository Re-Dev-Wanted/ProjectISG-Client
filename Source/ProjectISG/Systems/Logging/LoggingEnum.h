#pragma once
#include "LoggingEnum.generated.h"

UENUM()
enum class ELoggingActionType : uint8
{
	DAY_CYCLE,
	TIME_EVENT,
	INVENTORY,
	FARMING,
	HOUSING,
	COOKING,
	FISHING,
	TRADE,
	CRAFTING,
};

UENUM()
enum class ELoggingActionName : uint8
{
	start_day,
	sleep,

	// TIME_EVENT
	morning,
	afternoon,
	evening,
	night,

	// INVENTORY
	pickup_item,
	drop_item,
	equip_item,

	// FARMING
	plant_crop,
	water_crop,
	grow_crop,
	harvest_crop,

	// HOUSING
	place_housing,
	remove_housing,

	// COOKING
	start_cooking,
	progress_cooking,
	finish_cooking,

	// FISHING
	cast_bait,
	hook_bite,
	finish_fishing,

	// TRADE
	buy_item,
	sell_item,

	// CRAFTING
	craft_item
};
