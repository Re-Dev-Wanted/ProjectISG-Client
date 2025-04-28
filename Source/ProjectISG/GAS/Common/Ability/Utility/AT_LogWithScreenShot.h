#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "AT_LogWithScreenShot.generated.h"

UCLASS()
class PROJECTISG_API UAT_LogWithScreenShot : public UAbilityTask
{
	GENERATED_BODY()

public:
	static UAT_LogWithScreenShot* InitialEvent(UGameplayAbility* Ability);

	SETTER(FDiaryLogParams, LogParams)
	SETTER(bool, IsLogWithScreenShot)

protected:
	virtual void Activate() override;

private:
	FDiaryLogParams LogParams;
	bool IsLogWithScreenShot = false;
};
