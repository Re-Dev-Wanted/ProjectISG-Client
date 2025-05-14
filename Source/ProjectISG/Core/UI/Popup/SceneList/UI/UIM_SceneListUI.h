#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_SceneListUI.generated.h"

UCLASS()
class PROJECTISG_API UUIM_SceneListUI : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(FString, CurrentSceneId)
	GETTER_SETTER(uint8, CurrentLoadingPercent)
	GETTER(uint8, MaxLoadingPercent)

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta=(AllowPrivateAccess = true))
	FString CurrentSceneId;

	// 0 ~ 100까지 존재
	uint8 CurrentLoadingPercent = 0;

	uint8 MaxLoadingPercent = 100;
};
