#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionComponent.generated.h"

class UInputAction;

// 기타 기본 Input Action들을 담는 짬통 Actor Component로
// 움직임, 바라보기를 제외한 특정 Input Action이 필요하지만,
// 그것만을 위한 컴포넌트 만들기 조금 곤란하면 여기에 전부 몰아버리는 역할을 수행함
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UInputActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInputActionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

private:
	UFUNCTION()
	void BindingInputActions(
		UEnhancedInputComponent* EnhancedInputComponent);

#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ToggleQuestInputAction;

	UFUNCTION()
	void OnToggleQuestUI();

	UPROPERTY(EditDefaultsOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ToggleDiaryInputAction;

	UFUNCTION()
	void OnToggleDiaryUI();
#pragma endregion
};
