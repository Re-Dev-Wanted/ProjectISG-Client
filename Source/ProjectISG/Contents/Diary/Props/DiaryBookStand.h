#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "DiaryBookStand.generated.h"

UCLASS()
class PROJECTISG_API ADiaryBookStand
	: public ABaseActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ADiaryBookStand();

	virtual void OnInteractive(AActor* Causer) override;

	virtual FString GetDisplayText() const override;

protected:
	virtual void BeginPlay() override;
};
