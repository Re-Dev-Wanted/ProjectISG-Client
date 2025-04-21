#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainPlayerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UMainPlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMainPlayerComponent();

protected:
	virtual void InitializeComponent() override;

	UFUNCTION()
	virtual void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);
};
