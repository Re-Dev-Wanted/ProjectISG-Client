#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "Interfaces/IHttpRequest.h"

#include "DiaryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UDiaryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDiaryComponent();

	UFUNCTION()
	void GenerateDiary();

protected:
	virtual void BeginPlay() override;

private:
	FApiResponse GenerateDiaryResponse = {};
};
