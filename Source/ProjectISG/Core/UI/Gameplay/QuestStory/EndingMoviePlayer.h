// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingMoviePlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTISG_API UEndingMoviePlayer : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EndingNum = 1;
};
