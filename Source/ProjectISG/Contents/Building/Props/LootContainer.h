#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "LootContainer.generated.h"

UCLASS()
class PROJECTISG_API ALootContainer : public APlacement, public IItemHandler,
 public IUIHandler
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual void OnInteractive(AActor* Causer) override;

	virtual void OnInteractiveResponse(AActor* Causer) override;

	virtual bool GetCanInteractive() const override;

	virtual bool GetCanTouch() const override;

	virtual FString GetInteractiveDisplayText() const override;

	virtual bool ChangeItem(AActor* Causer, const FItemMetaInfo& ItemInfo, const uint16 Index) override;

	virtual void SwapItem(AActor* Causer, const uint16 Prev, const uint16 Next) override;

	virtual FItemMetaInfo GetItemMetaInfo(const uint16 Index) override;

	virtual void OnClosed() override;
	
	void UpdateItem(int32 Index, const FItemMetaInfo& NewItem);

	UFUNCTION(Server, Reliable)
	void Server_UpdateItem(int32 Index, const FItemMetaInfo_Net& NewItem);

	void SwapItemInternal(uint16 PrevIndex, uint16 NextIndex);

	UFUNCTION(Server, Reliable)
	void Server_SwapItem(const uint16 Prev, const uint16 Next);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly,
		meta = (AllowPrivateAccess = true, ClampMin = 1, ClampMax = 30))
	int32 Capacity = 25;

	UPROPERTY(Replicated)
	TArray<FItemMetaInfo_Net> Items;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Open;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SFX_Close;
};
