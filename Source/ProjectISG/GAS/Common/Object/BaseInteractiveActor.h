#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "ProjectISG/Systems/Input/Interface/InteractionInterface.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "BaseInteractiveActor.generated.h"

class AMainPlayerCharacter;

UCLASS()
class PROJECTISG_API ABaseInteractiveActor
	: public ABaseActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<AMainPlayerCharacter>, UsingCharacter)

	void SetUsingOwner(AMainPlayerCharacter* NewOwner);

protected:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Server, Reliable)
	void Server_SetUsingOwner(AMainPlayerCharacter* NewOwner);

	SETTER(TObjectPtr<AMainPlayerCharacter>, UsingCharacter)

	UPROPERTY(Replicated)
	TObjectPtr<AMainPlayerCharacter> UsingCharacter;
};
