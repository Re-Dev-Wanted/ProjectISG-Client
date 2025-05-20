#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Animation/AnimationData.h"
#include "Animation/AnimMontage.h"
#include "UObject/Object.h"
#include "AnimMontageManager.generated.h"

UCLASS()
class PROJECTISG_API UAnimMontageManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FAnimMontageData GetDataByKey(EAnimMontageKey Key);
	static TObjectPtr<UAnimMontage> GetMontage(EAnimMontageKey Key, 
	ECharacterName Character);

private:
	static TArray<FAnimMontageData> Datas;
	static TMap<EAnimMontageKey, FAnimMontageData> DataMap;
	static bool IsInitialized;
};
