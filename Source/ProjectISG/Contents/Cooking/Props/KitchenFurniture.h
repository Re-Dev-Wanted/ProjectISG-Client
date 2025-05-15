#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/GAS/Common/Object/BaseInteractiveActor.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "KitchenFurniture.generated.h"

enum class ECookingTool : uint8;
class UCameraComponent;
class UCookingComponent;

UCLASS()
class PROJECTISG_API AKitchenFurniture : public ABaseInteractiveActor
{
	GENERATED_BODY()

public:
	AKitchenFurniture();

	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	GETTER(TObjectPtr<UCameraComponent>, KitchenCameraComponent)

	virtual bool GetCanInteractive() const override;

	virtual FString GetInteractiveDisplayText() const override;

	void EquipCookingToolToAct(const FEquipCookingToolToActParams& Params);

	void UnEquipCookingToolToAct();

	UFUNCTION()
	void UnlockPlayer();

	UFUNCTION(Client, Reliable)
	void Client_UnlockPlayer();

	UFUNCTION(Server, Reliable)
	void Server_SetInteractingPlayer(class AMainPlayerCharacter* Player);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> KitchenStandPosition;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> KitchenCameraComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> FryPanMesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ScoopMesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WokMesh;
};
