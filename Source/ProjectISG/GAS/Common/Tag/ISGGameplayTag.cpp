#include "ISGGameplayTag.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"

namespace ISGGameplayTags
{
	void AddGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag
						, const int32 Count, const bool IsReplicated)
	{
		if (IsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, Count);
		}
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}

	void RemoveGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag
							, const int32 Count, const bool IsReplicated)
	{
		// Count가 -1 즉 기본 값으로 설정되어 있다면, 전부 없애기 위해 0으로 설정하고
		// 그게 아니라면 현재 갯수에서 N개만 빼게 설정한다.
		const int32 NewCount = Count == -1
									? 0
									: ASC->GetGameplayTagCount(Tag) - Count;

		if (IsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, NewCount);
		}
		ASC->SetLooseGameplayTagCount(Tag, NewCount);
	}

	void SetGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag
						, const int32 Count, const bool IsReplicated)
	{
		if (IsReplicated)
		{
			ASC->SetReplicatedLooseGameplayTagCount(Tag, Count);
		}
		ASC->SetLooseGameplayTagCount(Tag, Count);
	}

	void SwapGameplayTag(UAbilitySystemComponent* ASC
						, const FGameplayTag FromTag, const FGameplayTag ToTag
						, const bool IsReplicated)
	{
		const uint8 FromTagCount = ASC->GetGameplayTagCount(FromTag);
		RemoveGameplayTag(ASC, FromTag, FromTagCount, IsReplicated);
		AddGameplayTag(ASC, ToTag, FromTagCount, IsReplicated);
	}

	bool HasGameplayTag(UAbilitySystemComponent* ASC
						, const FGameplayTag FindToTag)
	{
		return ASC->HasMatchingGameplayTag(FindToTag);
	}

	void ToggleGameplayTag(UAbilitySystemComponent* ASC, const FGameplayTag Tag
							, const bool IsForceActive, const bool IsReplicated)
	{
		const bool HasTag = ASC->HasMatchingGameplayTag(Tag);
		// 강제 태그 
		const uint32 TagCount = IsForceActive ? IsForceActive : !HasTag;

		SetGameplayTag(ASC, Tag, TagCount, IsReplicated);
	}

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Test_Tag, "Test.Tag", "테스트용 노출 태그");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooking_Active_QTEAction
									, "Cooking.Active.QTEAction"
									, "요리 관련 액티브 : QTE 액션");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooking_Active_QTEEvent
									, "Cooking.Active.QTEEvent"
									, "요리 관련 액티브 : QTE 이벤트 시작");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cooking_Active_QTEEvent_KeyPressArray
									, "Cooking.Active.QTEEvent.KeyPressArray"
									, "요리 관련 액티브 : QTE 이벤트 시작 - 키 연속 입력");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Farming_Active_Seeding
									, "Farming.Active.Seeding"
									, "농사 관련 액티브 : 씨앗심기");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Farming_Active_Watering
									, "Farming.Active.Watering"
									, "농사 관련 액티브 : 물주기");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Farming_Active_Harvest
									, "Farming.Active.Harvest"
									, "농사 관련 액티브 : 수확하기");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Trading_Active_OpenTradingUI
									, "Trading.Active.OpenTradingUI"
									, "거래 관련 : 거래 창 열기");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Building_Active_Deconstruct
									, "Building.Active.Deconstruct"
									, "건설 관련 액티브 : 철거하기");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Building_Active_StartSitDown
									, "Building.Active.StartSitDown"
									, "의자 관련 액티브 : 앉기 시작");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Building_Active_NotifySitDown
									, "Building.Active.NotifySitDown"
									, "의자 관련 액티브 : 완전히 앉은 후 Notify");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Building_Active_EndSitDown
									, "Building.Active.EndSitDown"
									, "의자 관련 액티브 : 앉기 종료");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fishing_Active_CastBobber
									, "Fishing.Active.CastBobber"
									, "낚시 관련 액티브 : 낚시 찌 던지기");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fishing_Active_ReelInLine
									, "Fishing.Active.ReelInLine"
									, "낚시 관련 액티브 : 낚시 줄 감기");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sleeping_Active_LieInBed
									, "Sleeping.Active.LieInBed"
									, "수면 관련 액티브 : 눕기");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sleeping_Active_WakeUp
									, "Sleeping.Active.WakeUp"
									, "수면 관련 액티브 : 일어나기");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Interactive_Active_Exit
									, "Interactive.Active.Exit", "상호작용 상태 나가기");
}
