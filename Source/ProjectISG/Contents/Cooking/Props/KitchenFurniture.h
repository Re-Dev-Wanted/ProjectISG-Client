#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "KitchenFurniture.generated.h"

class AMainPlayerCharacter;
enum class ECookingTool : uint8;
class ULevelSequence;
class UCameraComponent;
class UCookingComponent;
class ALevelSequenceActor;
class ULevelSequencePlayer;

UCLASS()
class PROJECTISG_API AKitchenFurniture : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	AKitchenFurniture();

	virtual void OnInteractive(AActor* Causer) override;

	GETTER(TObjectPtr<UCameraComponent>, KitchenCameraComponent)

	virtual bool GetCanInteractive() const override;

	virtual FString GetInteractiveDisplayText() const override;

	void EquipCookingToolToAct(const FEquipCookingToolToActParams& Params);

	void UnEquipCookingToolToAct();

	void StartCookingMode();

	void EndCookingMode();

private:
	UPROPERTY()
	AMainPlayerCharacter* UsingCharacter;

#pragma region Cinematic
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> KitchenStandPosition;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> KitchenCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> StartCookingCinematic;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic",
		meta = (AllowPrivateAccess = true))
	TObjectPtr<ULevelSequence> EndCookingCinematic;

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> LevelSequencePlayer;

	UPROPERTY()
	ALevelSequenceActor* LevelSequenceActor;

	UFUNCTION()
	void OnFinishStartCooking();

	UFUNCTION()
	void OnFinishEndCooking();

	void LoggingStartCooking();

	void LoggingEndCooking();
#pragma endregion

#pragma region Mesh
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> FryPanMesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ScoopMesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WokMesh;
#pragma endregion
};
