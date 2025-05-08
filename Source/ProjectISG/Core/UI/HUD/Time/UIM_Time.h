// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIModel.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "UIM_Time.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UUIM_Time : public UBaseUIModel
{
	GENERATED_BODY()

public:
	GETTER(class UTexture2D*, MorningIcon)
	GETTER(class UTexture2D*, AfternoonIcon)
	GETTER(class UTexture2D*, EveningIcon)
	GETTER(class UTexture2D*, NightIcon)

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Time,
		meta = (AllowPrivateAccess))
	class UTexture2D* MorningIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Time,
		meta = (AllowPrivateAccess))
	class UTexture2D* AfternoonIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Time,
		meta = (AllowPrivateAccess))
	class UTexture2D* EveningIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Time,
		meta = (AllowPrivateAccess))
	class UTexture2D* NightIcon;
};
