#include "GA_BaseInputAbility.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"

UGA_BaseInputAbility::UGA_BaseInputAbility(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IsPassive = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_BaseInputAbility::OnAvatarSet(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	// AvatarSet이 해당 Ability가 처음 로드 될 때 동작한다.
	Super::OnAvatarSet(ActorInfo, Spec);
	SetupEnhancedInputBindings(ActorInfo, Spec);
}

void UGA_BaseInputAbility::SetupEnhancedInputBindings(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	// 우선 AvatarActor가 있어야 한다 보통 캐릭터가 이에 해당된다.
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		// AvatarActor가 기본 캐릭터 형태여야만 해당 시스템을 활용할 수 있다.
		const AMainPlayerCharacter* AvatarPawn = Cast<AMainPlayerCharacter>(
			ActorInfo->AvatarActor.Get());
		if (!AvatarPawn)
		{
			return;
		}

		const AController* PawnController = AvatarPawn->GetController();
		if (!PawnController)
		{
			return;
		}

		// EnhancedInput을 가져온다.
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
			UEnhancedInputComponent>(PawnController->InputComponent.Get()))
		{
			if (UGA_BaseInputAbility* AbilityInstance = Cast<
				UGA_BaseInputAbility>(Spec.Ability))
			{
				// Ability의 각각에 맞춘 Input Action이 존재하는 경우 트리거와 종료 시에 대한 설정을
				// 바인딩 처리 해준다.
				if (IsValid(AbilityInstance->ActivationInputAction))
				{
					EnhancedInputComponent->BindAction(
						ActivationInputAction, ETriggerEvent::Triggered,
						AbilityInstance
						, &ThisClass::OnAbilityInputPressed, ActorInfo);

					EnhancedInputComponent->BindAction(
						ActivationInputAction, ETriggerEvent::Completed,
						AbilityInstance
						, &ThisClass::OnAbilityInputReleased, ActorInfo);
				}
			}
		}
	}
}

// Ability가 종료될 때에 대한 기본적인 처리다.
void UGA_BaseInputAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
	                  , bWasCancelled);
	if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
	{
		if (const AController* PawnController = AvatarPawn->GetController())
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
				UEnhancedInputComponent>(PawnController->InputComponent.Get()))
			{
				// ID 정보만 없애준다. 사용 이후에는 Id를 비워줌으로써 나중에 다른 스킬로 토글될 때에 대한 대응이 가능해진다.
				EnhancedInputComponent->RemoveBindingByHandle(
					TriggeredEventHandle);
			}
		}
		TriggeredEventHandle = -1;
	}
}

void UGA_BaseInputAbility::BlockInputForMontage(bool value)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		CurrentActorInfo->AvatarActor.Get());
	if (Player)
	{
		Player->GetController()->SetIgnoreLookInput(value);
		Player->GetController()->SetIgnoreMoveInput(value);
	}
}

// Ability 자체가 제거될 때 기본적인 Binding된 정보를 제거해준다.
void UGA_BaseInputAbility::OnRemoveAbility(
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			if (const AController* PawnController = AvatarPawn->GetController())
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
					UEnhancedInputComponent>(PawnController->InputComponent))
				{
					EnhancedInputComponent->ClearBindingsForObject(
						Spec.Ability.Get());
				}
			}
		}
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}

// Ability의 시작 시 동작하는 함수
void UGA_BaseInputAbility::OnAbilityInputPressed(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const AMainPlayerCharacter* Owner = Cast<
		AMainPlayerCharacter>(ActorInfo->AvatarActor))
	{
		Owner->GetAbilitySystemComponent()->AbilityLocalInputPressed(
			static_cast<uint8>(InputId));
	}
}

// 키에서 손을 땔 때 돌아가는 함수로 홀딩에 필요한 경우 주로 사용할 수 있다.
void UGA_BaseInputAbility::OnAbilityInputReleased(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const AMainPlayerCharacter* Owner = Cast<
		AMainPlayerCharacter>(ActorInfo->AvatarActor))
	{
		Owner->GetAbilitySystemComponent()->AbilityLocalInputReleased(
			static_cast<uint8>(InputId));
	}
}
