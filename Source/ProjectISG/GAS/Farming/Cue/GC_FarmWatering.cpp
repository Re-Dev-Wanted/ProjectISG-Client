#include "GC_FarmWatering.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"

AGC_FarmWatering::AGC_FarmWatering()
{
	WateringEffect = CreateDefaultSubobject<UNiagaraComponent>(
		"Watering Effect");
}

void AGC_FarmWatering::BeginPlay()
{
	Super::BeginPlay();

	WateringEffect->Deactivate();
}

bool AGC_FarmWatering::OnExecute_Implementation(AActor* MyTarget,
                                                const FGameplayCueParameters&
                                                Parameters)
{
	SpawnSound();
	WateringEffect->Activate();

	if (const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		MyTarget))
	{
		// 손에 든 아이템이 있으면 그 아이템을 기준으로, 없으면 플레이어 기준으로 실행
		// 이펙트임으로 예외처리 수행
		const FVector TargetLocation = Player->GetInteractionComponent()->
		                                       GetTargetTraceResult().GetActor()
		                                       ->GetActorLocation() + FVector(
			0, 0, 40);

		// 현재 위치한 자리로 이동
		WateringEffect->SetWorldLocation(TargetLocation);
		AudioComponent->SetWorldLocation(TargetLocation);
		AudioComponent->Play();
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

bool AGC_FarmWatering::OnRemove_Implementation(AActor* MyTarget,
                                               const FGameplayCueParameters&
                                               Parameters)
{
	WateringEffect->Deactivate();
	AudioComponent->Stop();

	UE_LOG(LogTemp, Display, TEXT("테스트 Cue 제거 확인"))

	return Super::OnRemove_Implementation(MyTarget, Parameters);
}

void AGC_FarmWatering::SpawnSound()
{
	if (!WateringSound)
	{
		return;
	}

	if (!AudioComponent)
	{
		// 처음 한 번만 생성
		AudioComponent = NewObject<UAudioComponent>(this);
		AudioComponent->bAutoActivate = false;
		AudioComponent->bAutoDestroy = false;
		AudioComponent->RegisterComponent();

		AudioComponent->SetSound(WateringSound);
		AudioComponent->AttachToComponent(GetRootComponent(),
		                                  FAttachmentTransformRules::KeepRelativeTransform);
	}
}
