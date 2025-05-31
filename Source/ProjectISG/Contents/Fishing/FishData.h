#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UObject/Object.h"
#include "FishData.generated.h"

UENUM(BlueprintType)
enum class EFishGrade : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Mythic
};

USTRUCT(BlueprintType)
struct PROJECTISG_API FFishData : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(uint16, Id);
	GETTER(uint16, ItemId);
	GETTER(TObjectPtr<UStaticMesh>, Mesh);
	GETTER(EFishGrade, Grade);
	GETTER(float, WaitTimeMin)
	GETTER(float, WaitTimeMax)
	GETTER(FString, SocketName)

protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 Id = 0;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 ItemId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	EFishGrade Grade = EFishGrade::Common;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 2.f, ClampMax = 5.f, AllowPrivateAccess = true))
	float WaitTimeMin = 2.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 5.f, ClampMax = 20.f, AllowPrivateAccess = true))
	float WaitTimeMax = 5.f;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FString SocketName;

public:
	float GetWaitTime() const
	{
		return FMath::RandRange(WaitTimeMin, WaitTimeMax);
	}

	bool IsValid() const
	{
		return Id > 0 && ItemId > 0;
	}
};
