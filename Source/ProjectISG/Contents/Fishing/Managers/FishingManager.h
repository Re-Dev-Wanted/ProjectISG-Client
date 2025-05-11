#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Fishing/FishData.h"
#include "UObject/Object.h"
#include "FishingManager.generated.h"

UCLASS()
class PROJECTISG_API UFishingManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static TArray<FFishData> GetDatas()
	{
		return Datas;
	}

	static FFishData GetRandomData();

	static FFishData GetDataById(uint16 Id);
	static FFishData GetDataByItemId(uint16 ItemId);

private:
	static TArray<FFishData> Datas;
	static TMap<uint16, FFishData> DataMap;
	static bool IsInitialized;
};
