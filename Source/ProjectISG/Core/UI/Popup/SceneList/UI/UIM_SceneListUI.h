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
	GETTER_SETTER(uint8, CurrentSceneIndex)
	GETTER_SETTER(uint8, MaxSceneIndex)
	GETTER(uint8, MaxLoadingPercent)

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta=(AllowPrivateAccess = true))
	FString CurrentSceneId;

	uint8 CurrentSceneIndex = 0;

	uint8 MaxSceneIndex = 0;

	// 0 ~ 100까지 존재
	uint8 CurrentLoadingPercent = 0;

	uint8 MaxLoadingPercent = 100;
};
