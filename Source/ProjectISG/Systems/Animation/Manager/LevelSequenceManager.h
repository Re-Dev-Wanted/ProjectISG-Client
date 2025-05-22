#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "ProjectISG/Systems/Animation/AnimationData.h"
#include "UObject/Object.h"
#include "LevelSequenceManager.generated.h"

class AMainPlayerState;

UCLASS()
class PROJECTISG_API ULevelSequenceManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FLevelSequenceData GetDataByKey(ELevelSequenceKey Key);
	static TObjectPtr<ULevelSequence> GetLevelSequence(
		const AMainPlayerState* PS, const ELevelSequenceKey Key);

private:
	static TArray<FLevelSequenceData> Datas;
	static TMap<ELevelSequenceKey, FLevelSequenceData> DataMap;
	static bool IsInitialized;
};
