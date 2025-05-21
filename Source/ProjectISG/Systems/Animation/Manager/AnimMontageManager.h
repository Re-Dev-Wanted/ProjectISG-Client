#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Systems/Animation/AnimationData.h"
#include "Animation/AnimMontage.h"
#include "UObject/Object.h"
#include "AnimMontageManager.generated.h"

class AMainPlayerState;

UCLASS()
class PROJECTISG_API UAnimMontageManager : public UObject
{
	GENERATED_BODY()

public:
	static void Initialize();
	static FAnimMontageData GetDataByKey(const EAnimMontageKey Key);
	static TObjectPtr<UAnimMontage> GetMontage(const AMainPlayerState* PS,
	                                           const EAnimMontageKey Key);

private:
	static TArray<FAnimMontageData> Datas;
	static TMap<EAnimMontageKey, FAnimMontageData> DataMap;
	static bool IsInitialized;
};
