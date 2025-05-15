#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "QuestInteractiveActor.generated.h"

UCLASS()
class PROJECTISG_API AQuestInteractiveActor : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	AQuestInteractiveActor();

protected:
	virtual bool GetCanInteractive() const override { return true; }

	virtual FString GetInteractiveDisplayText() const override
	{
		return QuestInteractiveText;
	};

	virtual void OnInteractive(AActor* Causer) override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Options|Quest",
		meta = (AllowPrivateAccess = true))
	FString QuestInteractiveText;
};
