#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "Animation/AnimMontage.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "AnimationData.generated.h"

UENUM()
enum class EAnimMontageKey : uint8
{
	None,
	SitDown_Start,
	SitDown_Idle,
	SitDown_End,
	Farming_Watering,
	Sleep_Lying,
	Sleep_WakeUp,
};

UENUM()
enum class ELevelSequenceKey : uint8
{
	None,
};

UENUM()
enum class ECharacterName : uint8
{
	Default,
	Lumine
};

USTRUCT()
struct PROJECTISG_API FAnimMontageData : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(EAnimMontageKey, Key);

	FORCEINLINE TMap<ECharacterName, TObjectPtr<UAnimMontage>> GetMontageData
	() const
	{
		return MontageData;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta = (AllowPrivateAccess = true))
	EAnimMontageKey Key = EAnimMontageKey::None;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta = (AllowPrivateAccess = true))
	TMap<ECharacterName, TObjectPtr<UAnimMontage>> MontageData;
};


USTRUCT()
struct PROJECTISG_API FLevelSequenceData : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(ELevelSequenceKey, Key);

	FORCEINLINE TMap<ECharacterName, TObjectPtr<ULevelSequence>>
	GetSequenceData() const
	{
		return SequenceData;
	}

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta = (AllowPrivateAccess = true))
	ELevelSequenceKey Key = ELevelSequenceKey::None;

	UPROPERTY(EditDefaultsOnly, Category = "Data",
		meta = (AllowPrivateAccess = true))
	TMap<ECharacterName, TObjectPtr<ULevelSequence>> SequenceData;
};
