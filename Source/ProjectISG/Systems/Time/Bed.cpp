#include "Bed.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "SleepManager.h"
#include "TimeManager.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectISG/Contents/Diary/Component/DiaryComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Utils/EnumUtil.h"

ABed::ABed()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetBoxExtent(FVector(100.f, 130.f, 25.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeScale3D(FVector(0.f, 0.f, -30.f));
	// Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	WakeUpPos = CreateDefaultSubobject<USceneComponent>(TEXT("WakeUpPos"));
	WakeUpPos->SetupAttachment(Root);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(
		TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(Root);
	CameraComponent->SetRelativeLocation(
		FVector(0.000000, 250.000000, 300.000000));
	CameraComponent->SetRelativeRotation(FRotator(0.f, -90.f, -45.f));

	// ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(
	// 	TEXT("'/Game/Contents/Sleeping/Bed/Bed_Queen.Bed_Queen'"));
	// if (tempMesh.Succeeded())
	// {
	// 	Mesh->SetStaticMesh(tempMesh.Object);
	// }
	//
	// ConstructorHelpers::FObjectFinder<UMaterial> tempMat(
	// 	TEXT("'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	// if (tempMat.Succeeded())
	// {
	// 	Mesh->SetMaterial(0, tempMat.Object);
	// }

	Root->SetGenerateOverlapEvents(true);
}

void ABed::BeginPlay()
{
	Super::BeginPlay();


	TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));

	TimeManager->GetSleepManager()->WakeUpDelegate.AddDynamic(
		this, &ABed::ActivateWakeUp);
	TimeManager->GetSleepManager()->OpenDiaryDelegate.AddDynamic(
		this, &ABed::OpenDiary);
	TimeManager->GetSleepManager()->ForceSleepDelegate.AddDynamic(
		this, &ABed::ActivateSleepAbility);
}

void ABed::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CanInteractive);
	DOREPLIFETIME(ThisClass, MainPlayer);
}

void ABed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABed::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	UE_LOG(LogTemp, Warning, TEXT("침대 상호작용 함수 실행 , 로컬롤 : %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(GetLocalRole()));

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);

	FGameplayEventData EventPayload;
	EventPayload.EventTag = ISGGameplayTags::Sleeping_Active_LieInBed;
	EventPayload.Instigator = Player;
	EventPayload.Target = this;
	Player->GetAbilitySystemComponent()->HandleGameplayEvent(
		EventPayload.EventTag, &EventPayload);

	Player->GetInteractionComponent()->Server_OnInteractiveResponse(Causer);

	CanInteractive = false;
	Player->GetInteractionComponent()->SetIsInteractive(false);
	Player->GetPlacementIndicatorComponent()->SetIsActive(false);
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
		NetMulticast_MovePlayerToBed();
	}
}

FString ABed::GetInteractiveDisplayText() const
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
		AMainPlayerController* PC = Cast<AMainPlayerController>(
			MainPlayer->GetController());
		PC->SetViewTargetWithBlend(this);
		MainPlayer->SetActorRotation(WakeUpPos->GetComponentRotation());
		MainPlayer->SetActorLocation(WakeUpPos->GetComponentLocation());
	}
}

void ABed::NetMulticast_MovePlayerToBed_Implementation()
{
	MovePlayerToBed();
}

void ABed::ActivateWakeUp()
{
	if (MainPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("기상 어빌리티 강제 실행 로컬롤 : %s"),
		       *FEnumUtil::GetClassEnumKeyAsString(MainPlayer->GetLocalRole()));
		FGameplayTagContainer ActivateTag;
		ActivateTag.AddTag(ISGGameplayTags::Sleeping_Active_WakeUp);
		MainPlayer->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			ActivateTag);

		NetMulticast_InteractiveValue(true);
		SetCollisionEnabled(true);
		MainPlayer = nullptr;
	}
}

void ABed::OpenDiary()
{
	if (!MainPlayer)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("다이어리 작성 실행"));
	// 모든 침대에 대한 대응이 서버에서 발생하기 때문에
	// 로컬 컨트롤 상태 = 서버에서 통제중인 플레이어 이므로
	// 바로 함수 호출을 해도 문제가 없다.
	if (MainPlayer->IsLocallyControlled())
	{
		MainPlayer->GetDiaryComponent()->GenerateDiary();
	}
	else
	{
		// 그 외에는 Client 로 RPC 함수를 호출해서 실행시켜준다.
		MainPlayer->GetDiaryComponent()->Client_GenerateDiary();
	}
}

void ABed::ActivateSleepAbility()
{
	if (MainPlayer)
	{
		NetMulticast_MovePlayerToBed();
		UE_LOG(LogTemp, Warning, TEXT("수면 어빌리티 강제 실행 로컬롤 : %s"),
		       *FEnumUtil::GetClassEnumKeyAsString(MainPlayer->GetLocalRole()));
		FGameplayTagContainer ActivateTag;
		ActivateTag.AddTag(ISGGameplayTags::Sleeping_Active_LieInBed);
		MainPlayer->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(
			ActivateTag);
		NetMulticast_InteractiveValue(false);
	}
}

void ABed::SetCollisionEnabled(bool bEnable) const
{
	if (bEnable)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		Root->SetGenerateOverlapEvents(bEnable);
	}
	else
	{
		Root->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		Root->SetGenerateOverlapEvents(bEnable);
	}
}

void ABed::NetMulticast_InteractiveValue_Implementation(bool bEnable)
{
	CanInteractive = bEnable;

	MainPlayer->GetInteractionComponent()->SetIsInteractive(bEnable);
	MainPlayer->GetPlacementIndicatorComponent()->SetIsActive(bEnable);
}
