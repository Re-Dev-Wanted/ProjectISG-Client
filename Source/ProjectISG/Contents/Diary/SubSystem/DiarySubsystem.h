#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DiarySubsystem.generated.h"

UCLASS()
class PROJECTISG_API UDiarySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void GenerateDiary(const FApiRequestCallback& Callback,
	                   FApiResponse& Response);
};
