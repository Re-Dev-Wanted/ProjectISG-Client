#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "LootContainer.generated.h"

UCLASS()
class PROJECTISG_API ALootContainer : public APlacement
{
	GENERATED_BODY()

public:
	virtual void SetOption(bool bIsGhost, bool bIsBlock = false) override;
	
	virtual void OnInteractive(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual FString GetInteractiveDisplayText() const override;

protected:
	// 아이템 데이터를 저장하거나 얻기 위한 고유 Guid
	UPROPERTY()
	FGuid Guid;

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, ClampMin = 1, ClampMax = 30))
	int32 Capacity;
};
