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
	
	CropStartGrowTime = (TimeManager->GetHour() * 3600 + TimeManager->GetMinute() * 60 + TimeManager->GetSecond());

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
	// 초단위
	float CropCurrentGrowTime = TimeManager->GetHour() * 3600 + TimeManager->GetMinute() * 60 + TimeManager->GetSecond(); 
	float CropGrowTime = CropCurrentGrowTime - CropStartGrowTime;

	// 초단위를 3600으로 나누어 시간단위로 계산
	if (CropGrowTime / 3600 >= CropTotalGrowTime)
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


