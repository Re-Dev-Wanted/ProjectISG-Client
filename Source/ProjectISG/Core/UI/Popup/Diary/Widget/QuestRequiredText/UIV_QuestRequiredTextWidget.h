#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIView.h"
#include "UIV_QuestRequiredTextWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTISG_API UUIV_QuestRequiredTextWidget : public UBaseUIView
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UTextBlock>, RequireDescription)
	GETTER(TObjectPtr<UTextBlock>, RequireStatus)

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequireDescription;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RequireStatus;
};
