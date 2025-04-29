#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "InteractionComponent.generated.h"

class AMainPlayerCharacter;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	GETTER(FHitResult, TargetTraceResult)

	void SetIsInteractive(const bool NewIsInteractive);

	UFUNCTION()
	void OnChange(TSubclassOf<AActor> ActorClass, FItemMetaInfo _ItemMetaInfo);

	UFUNCTION(Reliable, Server)
	void Server_Interact(class ABaseInteractiveActor* InteractActor);

	UFUNCTION(Reliable, Server)
	void Server_Touch(class ABaseInteractiveActor* InteractActor);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Input",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> TouchAction;

	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);

	UFUNCTION()
	void OnInteractive();

	UFUNCTION()
	void OnTouch();
#pragma endregion

#pragma region Target
	UPROPERTY()
	TObjectPtr<AMainPlayerCharacter> PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true))
	uint16 TargetRange = 300;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Targeting",
		meta = (AllowPrivateAccess = true, ClampMin = "0", ClampMax = "100"))
	uint8 TargetRadius = 36;

	UPROPERTY()
	FHitResult TargetTraceResult;

	void LineTraceToFindTarget();

	// 현재 상호작용 중 인지 알 수 있는 flag 값으로
	// 해당 flag가 비활성화 상태인 경우는 Trace가 되지 않는 상태가 된다.
	bool IsInteractive;
#pragma endregion
};
