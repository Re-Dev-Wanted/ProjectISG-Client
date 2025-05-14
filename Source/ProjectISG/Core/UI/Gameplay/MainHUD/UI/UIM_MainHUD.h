// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIM_MainHUD : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER_SETTER(FString, CurrentPlayingAutoQuestId)

private:
	// Quest Manage Component에도 존재하는 하지만 혹시 모를 값 저장은 해둔다.
	// 실제로는 Quest Manage Component에서 값을 관리한다.
	FString CurrentPlayingAutoQuestId;
};
