// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCrop.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Time/TimeManager.h"


// Sets default values
ABaseCrop::ABaseCrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);


	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ABaseCrop::BeginPlay()
{
	Super::BeginPlay();

	TimeManager = Cast<ATimeManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATimeManager::StaticClass()));
	if (!TimeManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("타임 매니저가 없습니다"));
		return;
	}

	CropStartGrowDay = TimeManager->GetDay();
	CropStartGrowTime = (TimeManager->GetHour()) + (TimeManager->GetMinute() / 60) + (TimeManager->GetSecond() / 3600);

	// 2초에 한번씩 다 자랐는지 체크
	TWeakObjectPtr<ABaseCrop> weakThis = this;
	GetWorld()->GetTimerManager().SetTimer(GrowTimerHandle, [weakThis]()
	{
		if (weakThis.IsValid())
		{
			weakThis.Get()->CheckGrowTime();
		}
	}, 2.f, true);

	Root->OnComponentBeginOverlap.AddDynamic(this, &ABaseCrop::CropBeginOverlap);
}

void ABaseCrop::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCrop, bIsMature);
}

// Called every frame
void ABaseCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCrop::CheckGrowTime()
{
	// 시간단위로 바꿔서 현재 월드의 시간을 구한다.
	float CropCurrentGrowDay = TimeManager->GetDay() - CropStartGrowDay;
	float CropCurrentGrowTime = (CropCurrentGrowDay * 24) + (TimeManager->GetHour()) + (TimeManager->GetMinute() / 60) + (TimeManager->GetSecond() / 3600);

	// 현재 시간과 씨앗을 심은 시간을 빼서 현재 지난 시간을 구한다.
	float CropGrowTime = CropCurrentGrowTime - CropStartGrowTime;

	// 농작물의 성장 시간 변수와 비교한다
	if (CropGrowTime >= CropTotalGrowTime)
	{
		bIsMature = true;
		UE_LOG(LogTemp, Warning, TEXT("다 자랐다"));
		GetWorld()->GetTimerManager().ClearTimer(GrowTimerHandle);
	}
}

void ABaseCrop::CropBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsMature == false) return;

	AMainPlayerCharacter* player = Cast<AMainPlayerCharacter>(OtherActor);
	if (player)
	{
		UE_LOG(LogTemp, Warning, TEXT("캐짐"));
		Destroy();
	}
}


