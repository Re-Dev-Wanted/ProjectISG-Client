#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInventoryComponent.generated.h"

class UInputAction;
class UEnhancedInputComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UPlayerInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	bool IsOpenedInventory = false;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ToggleInventoryInputAction;

	UFUNCTION()
	void BindingInputActions(UEnhancedInputComponent* EnhancedInputComponent);

	void ToggleInventory();
};
