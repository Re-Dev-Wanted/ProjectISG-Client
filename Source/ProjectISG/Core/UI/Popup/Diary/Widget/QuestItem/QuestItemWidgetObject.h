#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestItemWidgetObject.generated.h"

UCLASS()
class PROJECTISG_API UQuestItemWidgetObject : public UObject
{
	GENERATED_BODY()

public:
	FString QuestId;
};
