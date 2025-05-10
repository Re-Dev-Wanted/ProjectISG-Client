#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KitchenFurnitureCinematicComponent.generated.h"

class AMainPlayerCharacter;
class AKitchenFurniture;
class ULevelSequence;
class UCameraComponent;
class UCookingComponent;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API UKitchenFurnitureCinematicComponent
	: public UActorComponent
{
	GENERATED_BODY()

public:
	UKitchenFurnitureCinematicComponent();

	void StartCookingMode(AMainPlayerCharacter* Target);
	void EndCookingMode(AMainPlayerCharacter* Target);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<AKitchenFurniture> Kitchen;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> StartCookingCinematic;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> EndCookingCinematic;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	// 시네마틱을 위해 전달하는 임시 변수로
	// 실제로 활용되지는 않는다고 생각하면 편하다.
	UPROPERTY()
	AMainPlayerCharacter* SelectedTarget;

	UFUNCTION()
	void OnFinishStartCooking();

	UFUNCTION()
	void OnFinishEndCooking();

	void LoggingStartCooking() const;

	void LoggingEndCooking() const;
};
