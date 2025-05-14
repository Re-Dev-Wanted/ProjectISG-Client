#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ISGGameInstance.generated.h"

UCLASS()
class PROJECTISG_API UISGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UISGGameInstance();

protected:
	virtual void Init() override;
};
