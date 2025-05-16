#pragma once
#include "ProjectISG/Utils/MacroUtil.h"
#include "FarmingStruct.generated.h"

USTRUCT(BlueprintType)
struct PROJECTISG_API FFarmingStruct : public FTableRowBase
{
	GENERATED_BODY()

	GETTER(uint16, CropId)
	GETTER(uint16, ItemId)
	GETTER(FVector, Scale)
	GETTER(FVector, Location)
	GETTER(TObjectPtr<UStaticMesh>, StaticMesh)
	GETTER(TObjectPtr<UMaterialInstance>, MeshMaterial)
	GETTER(int32, Yield)

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 CropId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint16 ItemId = 0;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FVector Scale = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterialInstance> MeshMaterial;

	// 수확량
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	int32 Yield = 0;
};
