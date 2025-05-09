#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "InteractionComponent.generated.h"

class ABaseInteractiveActor;
class AMainPlayerCharacter;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	GETTER(FHitResult, TargetTraceResult)
	GETTER_SETTER(TObjectPtr<ABaseInteractiveActor>, SelectedInteractiveActor)

	void SetIsInteractive(const bool NewIsInteractive);

	UFUNCTION()
	void OnChange(uint16 ItemId);

	UFUNCTION(Reliable, Server)
	void Server_Interact(ABaseInteractiveActor* InteractActor, AActor*
	                     Causer);

	UFUNCTION(Reliable, Server)
	void Server_Touch(ABaseInteractiveActor* InteractActor, AActor*
	                  Causer);

	// 하드 코딩 방지를 위해 해당 actor class를 BaseActor로 받아서
	// 따로 조건 처리를 하는게 맞아보인다.
	UFUNCTION(Server, Reliable)
	void Server_OnInteractiveResponse(AActor* Causer);

	UFUNCTION(Server, Reliable)
	void Server_OnTouchResponse(AActor* Causer);

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;
	void ToggleInteractiveUI();

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

	// 해당 변수 값의 경우 상호작용 중인 현재 액터 값을 저장하는 곳이기 때문에
	// Trace 된다고 바로 저장하는 변수 값은 아니다. 사용에 주의할 것
	UPROPERTY()
	TObjectPtr<ABaseInteractiveActor> SelectedInteractiveActor;

	void LineTraceToFindTarget();

	// 현재 상호작용 중 인지 알 수 있는 flag 값으로
	// 해당 flag가 비활성화 상태인 경우는 Trace가 되지 않는 상태가 된다.
	bool IsInteractive;
#pragma endregion
};
