#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "KitchenFurniture.generated.h"

class UKitchenFurnitureCinematicComponent;
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

	void PlayCookingCinematic(AMainPlayerCharacter* Target
							, const EKitchenFurnitureCinematicStatus Status);

	UFUNCTION(Server, Unreliable)
	void Server_PlayCookingCinematic(AMainPlayerCharacter* Target
									, const EKitchenFurnitureCinematicStatus
									Status);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayCookingCinematic(AMainPlayerCharacter* Target
										, const EKitchenFurnitureCinematicStatus
										Status);

private:
#pragma region Cinematic
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> KitchenStandPosition;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> KitchenCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UKitchenFurnitureCinematicComponent>
	KitchenFurnitureCinematicComponent;

	void PlayCookingCinematic_Internal(AMainPlayerCharacter* Target
										, const EKitchenFurnitureCinematicStatus
										Status);
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
