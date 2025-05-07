#include "Bed.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "SleepManager.h"
#include "TimeManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

ABed::ABed()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(110.f, 55.f, 32.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeScale3D(FVector(2.f, 1.f, 0.5f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	WakeUpPos = CreateDefaultSubobject<USceneComponent>(TEXT("WakeUpPos"));
	WakeUpPos->SetupAttachment(Root);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(
		TEXT("'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(
		TEXT("'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (tempMat.Succeeded())
	{
		Mesh->SetMaterial(0, tempMat.Object);
	}

	Root->SetGenerateOverlapEvents(true);
}

void ABed::BeginPlay()
{
	Super::BeginPlay();


	TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));

	TimeManager->GetSleepManager()->SleepDelegate.AddDynamic(
		this, &ABed::MovePlayerToBed);
	TimeManager->GetSleepManager()->WakeUpDelegate.AddDynamic(
		this, &ABed::MovePlayerToWakeUpPos);
}

void ABed::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CanInteractive);
}

void ABed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABed::OnInteractive(AActor* Causer)
{
	Super::OnInteractive(Causer);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);

	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Sleeping_Active_LieInBed);

	CanInteractive = false;
	Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ActivateTag);
}

void ABed::OnInteractiveResponse(AActor* Causer)
{
	IInteractionInterface::OnInteractiveResponse(Causer);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	if (Player)
	{
		Player->SetbLieInBed(true);
		if (HasAuthority())
		{
			MainPlayer = Player;
		}
		Server_MovePlayerToBed();
	}
}

FString ABed::GetDisplayText() const
{
	return TEXT("눕기");
}

bool ABed::GetCanInteractive() const
{
	return CanInteractive;
}

void ABed::MovePlayerToBed()
{
	if (MainPlayer)
	{
		MainPlayer->SetActorLocation(GetActorLocation());
	}
}

void ABed::Server_MovePlayerToBed_Implementation()
{
	NetMulticast_MovePlayerToBed();
}

void ABed::NetMulticast_MovePlayerToBed_Implementation()
{
	MovePlayerToBed();
}

void ABed::MovePlayerToWakeUpPos()
{
	if (MainPlayer)
	{
		MainPlayer->SetActorLocation(WakeUpPos->GetComponentLocation());
	}
}
