#pragma once

#include "NativeGameplayTags.h"

struct FGameplayTag;
class UAbilitySystemComponent;

namespace ISGGameplayTags
{
	PROJECTISG_API void AddGameplayTag(UAbilitySystemComponent* ASC
	                                   , const FGameplayTag Tag
	                                   , const int32 Count = 1
	                                   , const bool IsReplicated = false);
	PROJECTISG_API void RemoveGameplayTag(UAbilitySystemComponent* ASC
	                                      , const FGameplayTag Tag
	                                      , const int32 Count = -1
	                                      , const bool IsReplicated = false);
	PROJECTISG_API void SetGameplayTag(UAbilitySystemComponent* ASC
	                                   , const FGameplayTag Tag
	                                   , const int32 Count
	                                   , const bool IsReplicated = false);
	PROJECTISG_API void SwapGameplayTag(UAbilitySystemComponent* ASC
	                                    , const FGameplayTag FromTag
	                                    , const FGameplayTag ToTag
	                                    , const bool IsReplicated = false);
	PROJECTISG_API bool HasGameplayTag(UAbilitySystemComponent* ASC
	                                   , const FGameplayTag FindToTag);
	PROJECTISG_API void ToggleGameplayTag(UAbilitySystemComponent* ASC
	                                      , const FGameplayTag Tag
	                                      , const bool IsForceActive = false
	                                      , const bool IsReplicated = false);

	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Test_Tag)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooking_Active_StartCooking)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooking_Active_EndCooking)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooking_Active_QTEAction)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cooking_Active_QTEEvent)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(
		Cooking_Active_QTEEvent_KeyPressArray)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Farming_Active_Seeding)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Farming_Active_Watering)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Farming_Active_Harvest)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Trading_Active_OpenTradingUI)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Building_Active_Deconstruct)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Building_Active_StartSitDown)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Building_Active_NotifySitDown)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Building_Active_EndSitDown)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fishing_Active_CastBobber)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fishing_Active_ReelInLine)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sleeping_Active_LieInBed)
	PROJECTISG_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sleeping_Active_WakeUp)
}
