#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectISG/Core/ISGGameInstance.h"
#include "UI_BaseButton.generated.h"

enum class EColorPalette : uint8;
class UTextBlock;
class UButton;

UCLASS()
class PROJECTISG_API UUI_BaseButton : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UButton> Get() { return ParentButton; }
	GETTER(TObjectPtr<UTextBlock>, Text)

protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ParentButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(EditAnywhere, Category = "Options"
		, meta=(AllowPrivateAccess = true))
	FText TextValue = FText::FromString(TEXT("Test Name"));
};
