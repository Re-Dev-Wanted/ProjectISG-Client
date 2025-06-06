#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "TradingTabButton.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class PROJECTISG_API UTradingTabButton : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, Button)

	void OnActive(bool IsActive);

protected:
	virtual void NativePreConstruct() override;
	
	void SetStyleInternal(FSlateColor ButtonColor, FSlateColor TextColor);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector4 Corner = FVector4::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ActiveColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor DeactiveColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor ActiveTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor DeactiveTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;	
};
