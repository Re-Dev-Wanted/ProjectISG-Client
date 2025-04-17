#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "MainPlayerState.generated.h"

class UInventoryComponent;
class UISGAbilitySystemComponent;

UCLASS()
class PROJECTISG_API AMainPlayerState : public APlayerState,
                                        public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMainPlayerState();
	void InitializeData();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	GETTER(TObjectPtr<UInventoryComponent>, InventoryComponent)

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UISGAbilitySystemComponent> AbilitySystemComponent;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
};
