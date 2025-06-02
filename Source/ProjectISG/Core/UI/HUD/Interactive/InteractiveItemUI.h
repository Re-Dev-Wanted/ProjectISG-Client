// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractiveItemUI.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class PROJECTISG_API UInteractiveItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractive(const FString& Key, const FString& Text) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> KeyIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisplayText;

	UPROPERTY(EditAnywhere)
	TMap<FString, TSoftObjectPtr<UTexture2D>> IconMap;
};
