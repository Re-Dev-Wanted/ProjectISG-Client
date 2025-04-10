#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ISGGameInstance.generated.h"

UCLASS()
class PROJECTISG_API UISGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UISGGameInstance();

	FORCEINLINE TArray<FItemInfoData> GetItemInfoList() const
	{
		return ItemInfoList;
	}

private:
	void LoadItemData();

	UPROPERTY()
	TArray<FItemInfoData> ItemInfoList;
};
