#include "PlacementIndicatorComponent.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerInventoryComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Manager/GridManager.h"
#include "ProjectISG/Systems/Inventory/Components/InventoryComponent.h"
#include "ProjectISG/Systems/Inventory/Managers/ItemManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

class AMainPlayerCharacter;

UPlacementIndicatorComponent::UPlacementIndicatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> InputAction(
		TEXT(
			"/Game/Core/Character/Blueprints/MainPlayer/Input/IA_RotateObject.IA_RotateObject"));

	if (InputAction.Succeeded())
	{
		RotateAction = InputAction.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> InputAction2(
		TEXT(
			"/Game/Core/Character/Blueprints/MainPlayer/Input/IA_Touch.IA_Touch"));

	if (InputAction2.Succeeded())
	{
		TouchAction = InputAction2.Object;
	}
}

void UPlacementIndicatorComponent::InitializePlaceIndicator()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	Player = Cast<AMainPlayerCharacter>(GetOwner());
	
	if (!Player)
	{
		return;
	}

	const AMainPlayerState* PS = Player->GetPlayerState<AMainPlayerState>();

	Player->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);

	if (PS)
	{
		PS->GetInventoryComponent()
				   ->OnInventoryUpdateNotified.AddDynamic(
					   this, &ThisClass::OnInventoryUpdated);
		const FItemMetaInfo ItemMetaInfo = PS->GetInventoryComponent()
					->GetInventoryList()[0];
		
		SetVisibilityInternal(ItemMetaInfo.GetId());
	}

}

void UPlacementIndicatorComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered,
	                                   this, &ThisClass::OnRotate);
	EnhancedInputComponent->BindAction(TouchAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::Execute);
}

void UPlacementIndicatorComponent::TickComponent(
	float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsActive)
	{
		return;
	}

	if (!bIsIndicatorActive)
	{
		return;
	}

	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}

	LineTrace();
}

void UPlacementIndicatorComponent::Execute()
{
	if (!bIsTraced)
	{
		return;
	}
	
	if (!IsActive)
	{
		return;
	}

	if (!bIsIndicatorActive)
	{
		return;
	}

	if (bIsBlocked)
	{
		return;
	}

	if (!IndicateActor)
	{
		return;
	}

	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}

	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
		PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}

	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}

	const TObjectPtr<UPlayerInventoryComponent> PlayerInventoryComponent =
		Player->GetPlayerInventoryComponent();

	FRotator Rotation = GridManager->GetSnappedRotation
		(GetDegrees(RotateDirection));

	if (GetOwner()->HasAuthority())
	{
		ExecuteInternal(IndicateActor->GetActorPivotLocation(),
		                IndicateActor->GetActorLocation(), Rotation,
		                IndicateActor->GetClass(), PlacementItemId);
	}
	else
	{
		Server_Execute(IndicateActor->GetActorPivotLocation(),
		               IndicateActor->GetActorLocation(), Rotation,
		               IndicateActor->GetClass(), PlacementItemId);
	}

	if (!bCanGeneratedOtherItem)
	{
		if (PlayerInventoryComponent->RemoveItemCurrentSlotIndex(1))
		{
			OnDeactivate();
		}
	}
}

void UPlacementIndicatorComponent::ExecuteInternal(
	FVector Pivot, FVector Location, FRotator Rotation,
	TSubclassOf<APlacement> PlacementClass, uint16 ItemId)
{
	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}

	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
		PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}

	AGridManager* GridManager = PlayerState->GetGridManager();

	if (GridManager)
	{
		if (GetOwner()->HasAuthority())
		{
			GridManager->BuildPlacement(PlacementClass, ItemId, Pivot, Location,
			                            Rotation);

			// Logging
			if (UItemManager::GetItemUsingType(ItemId) != "Disposability")
			{
				FDiaryLogParams LogParams;
				LogParams.Location = TEXT("농장");
				LogParams.ActionType = ELoggingActionType::HOUSING;
				LogParams.ActionName = ELoggingActionName::place_housing;

				GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()
				          ->
				          LoggingDataWithScreenshot(LogParams);
			}

			// 제단 설치 퀘스트 코드
			AMainPlayerController* PC = Cast<AMainPlayerController>(PlayerController);
			if (PC)
			{
				if (PC->GetQuestManageComponent()->GetCurrentPlayingQuestId() == FString::Printf(TEXT("Story_005")))
				{
					PC->SetCustomQuestComplete(true);
				}
			}
		}
		else
		{
			GridManager->Server_BuildPlacement(PlacementClass, ItemId, Pivot,
			                                   Location, Rotation);
		}
	}
}

void UPlacementIndicatorComponent::LineTrace()
{
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
		PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}

	AGridManager* GridManager = PlayerState->GetGridManager();

	if (!GridManager)
	{
		return;
	}

	const TArray<AActor*> IgnoreActors;
	const FVector OwnerStartLocation = Player->GetActorLocation();
	const FVector OwnerEndLocation = OwnerStartLocation + Player->
		GetCameraComponent()->GetForwardVector() * TargetRange;

	const AMainPlayerController* PC = Cast<AMainPlayerController>(
		PlayerController);

	if (!PC)
	{
		return;
	}

	bIsTraced = UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(),
		OwnerStartLocation,
		OwnerEndLocation, TargetRadius,
		TargetRadius, TraceTypeQuery1,
		false, IgnoreActors,
		EDrawDebugTrace::None,
		TargetTraceResult, true);

	if (IndicateActor)
	{
		IndicateActor->SetActorHiddenInGame(!bIsTraced);
	}

	if (bIsTraced)
	{
		if (PlayerController->GetCharacter()->IsLocallyControlled())
		{
			FVector SnappedLocation = GridManager->SnapToGridPlacement(
				TargetTraceResult.ImpactPoint);
			FRotator SnappedRotation = GridManager->GetSnappedRotation
				(GetDegrees(RotateDirection));

			if (!IndicateActor)
			{
				return;
			}

			IndicateActor->SetActorLocation(FMath::VInterpTo(
				IndicateActor->GetActorLocation(), SnappedLocation,
				0.1f,
				InterpSpeed));

			IndicateActor->SetActorRotation(FMath::RInterpTo(
				IndicateActor->GetActorRotation(),
				FRotator(0,
				         SnappedRotation.Yaw,
				         0), 0.1f,
				InterpSpeed));

			bIsBlocked = !GridManager->IsEmptyGrid(SnappedLocation);

			IndicateActor->SetOption(true, bIsBlocked);

			if (PC->GetMainHUD())
			{
				PC->GetMainHUD()->TogglePlacementIndicatorUI(!bIsBlocked);
			}
		}
	}
	else
	{
		if (PC->GetMainHUD())
		{
			PC->GetMainHUD()->TogglePlacementIndicatorUI(false);
		}
	}
}

void UPlacementIndicatorComponent::OnRotate(
	const FInputActionValue& InputActionValue)
{
	if (!bIsTraced)
	{
		return;
	}
	
	if (!IsActive)
	{
		return;
	}

	if (!bIsIndicatorActive)
	{
		return;
	}

	float Value = InputActionValue.Get<float>();

	if (Value > 0)
	{
		RotateDirection = RotateDirection << static_cast<uint8>(1);
	}

	if (Value < 0)
	{
		RotateDirection = RotateDirection >> static_cast<uint8>(1);
	}
}

void UPlacementIndicatorComponent::OnChange(
	uint16 ItemId)
{
	RotateDirection = North;

	SetVisibilityInternal(ItemId);
}

void UPlacementIndicatorComponent::FindGridManagerInternal(AGridManager*& GridManager)
{
	for (TActorIterator<AGridManager> It(GetWorld()); It; ++It)
	{
		AGridManager* Found = *It;
		if (Found)
		{
			GridManager = Found;
			break;
		}
	}
}

void UPlacementIndicatorComponent::OnActivate(
	const TSubclassOf<APlacement>& Factory)
{
	if (!PlayerController || !PlayerController->PlayerState)
	{
		return;
	}

	const AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
		PlayerController->PlayerState);

	if (!PlayerState)
	{
		return;
	}

	if (PlayerController->GetCharacter()->IsLocallyControlled())
	{
		AGridManager* GridManager = PlayerState->GetGridManager();

		if (!GridManager)
		{
			FindGridManagerInternal(GridManager);

			if (!GridManager)
			{
				return;
			}
		}

		GridManager->SetVisibleGrid(true);

		SetActive(true);

		if (IndicateActor)
		{
			APlacement* DestroyActor = IndicateActor.Get();
			DestroyActor->Destroy();
			IndicateActor = nullptr;
		}

		IndicateActor = GetWorld()->SpawnActor<APlacement>(Factory);

		if (IndicateActor)
		{
			IndicateActor->SetReplicates(false); // 복제 금지
			IndicateActor->SetActorEnableCollision(false); // 충돌 제거
			IndicateActor->SetActorTickEnabled(false);
			IndicateActor->Setup(GridManager->SnapSize);
			IndicateActor->SetGuide(GridManager->SnapSize);
		}

		const AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerController);

		if (PC && PC->GetMainHUD())
		{
			PC->GetMainHUD()->TogglePlacementIndicatorUI(true);
		}
	}
}

void UPlacementIndicatorComponent::OnDeactivate()
{
	if (Player && Player->IsLocallyControlled())
	{
		if (IndicateActor)
		{
			APlacement* DestroyActor = IndicateActor.Get();
			DestroyActor->Destroy();
			IndicateActor = nullptr;
		}

		SetActive(false);

		if (PlayerController)
		{
			AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
				PlayerController->PlayerState);

			if (!PlayerState)
			{
				return;
			}

			AGridManager* GridManager = PlayerState->GetGridManager();

			if (!GridManager)
			{
				FindGridManagerInternal(GridManager);
			}

			if (GridManager)
			{
				GridManager->SetVisibleGrid(false);
			}
		}

		const AMainPlayerController* PC = Cast<AMainPlayerController>(
			PlayerController);

		if (PC && PC->GetMainHUD())
		{
			PC->GetMainHUD()->TogglePlacementIndicatorUI(false);
		}

		Player->GetInteractionComponent()->SetIsInteractive(true);
	}
}

void UPlacementIndicatorComponent::SetIsActive(bool NewActive)
{
	IsActive = NewActive;
	AMainPlayerState* PlayerState = Cast<AMainPlayerState>(
		PlayerController->PlayerState);

	AGridManager* GridManager = PlayerState->GetGridManager();

	if (GridManager)
	{
		GridManager->SetVisibleGrid(NewActive);
	}

	if (IndicateActor)
	{
		IndicateActor->SetActorHiddenInGame(!NewActive);
	}
}

void UPlacementIndicatorComponent::Server_Execute_Implementation(FVector Pivot,
	FVector Location, FRotator Rotation, TSubclassOf<APlacement> PlacementClass,
	uint16 ItemId)
{
	ExecuteInternal(Pivot, Location, Rotation, PlacementClass, ItemId);
}

void UPlacementIndicatorComponent::OnInventoryUpdated()
{
	if (!Player)
	{
		return;
	}
	
	const uint32 CurrentIndex = Player->GetPlayerInventoryComponent()->
											 GetCurrentSlotIndex();

	const FItemMetaInfo ItemMetaInfo = Player->
									   GetPlayerState<AMainPlayerState>()->
									   GetInventoryComponent()->
									   GetInventoryList()[CurrentIndex];
	
	SetVisibilityInternal(ItemMetaInfo.GetId());
}

void UPlacementIndicatorComponent::SetVisibilityInternal(uint16 ItemId)
{
	const FItemInfoData ItemInfoData = UItemManager::GetItemInfoById(ItemId);

	const bool bIsStructure = UItemManager::IsItemCanHousing(ItemId);

	const TSubclassOf<AActor> ActorClass = ItemInfoData.GetPlaceItemActor();

	bIsIndicatorActive = bIsStructure
		&& ActorClass
		&& ActorClass->IsChildOf(APlacement::StaticClass());

	// 하우징이 가능한 상태
	if (bIsIndicatorActive)
	{
		const uint16 GeneratedOtherItemId =
			UItemManager::GetGeneratedOtherItemIdById(ItemId);
		// 다른 아이템을 생성하는 도구인지 판단

		// 다른 아이템을 생성한다 -> 특정 아이템을 건축한다
		// Item Type이 Housing이고, 생성되는 아이템이 공기가 아니다.
		bCanGeneratedOtherItem = GeneratedOtherItemId > 0;

		uint16 OtherItemId = 0;
		TSubclassOf<APlacement> PlacementClass;

		if (bCanGeneratedOtherItem)
		{
			const FItemInfoData OtherInfoData = UItemManager::GetItemInfoById(
				GeneratedOtherItemId);

			if (UItemManager::IsItemCanHousing(ItemId))
			{
				const TSubclassOf<AActor> OtherActorClass = OtherInfoData.
					GetPlaceItemActor();
				if (OtherActorClass && OtherActorClass->IsChildOf(
					APlacement::StaticClass()))
				{
					PlacementClass = OtherActorClass;
					OtherItemId = GeneratedOtherItemId;
				}
			}
		}

		PlacementItemId = bCanGeneratedOtherItem && OtherItemId > 0
							  ? OtherItemId
							  : ItemId;
		
		if (!PlacementClass)
		{
			PlacementClass = ActorClass;
		}

		OnActivate(PlacementClass);
	}
	else
	{
		PlacementItemId = 0;
		OnDeactivate();
	}
}
