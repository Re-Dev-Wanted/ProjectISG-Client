#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "DiaryBookStand.generated.h"

UCLASS()
class PROJECTISG_API ADiaryBookStand : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	ADiaryBookStand();

	virtual void OnInteractive(AActor* Causer) override;

	virtual FString GetDisplayText() const override;

	virtual bool GetCanInteractive() const override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;
};
