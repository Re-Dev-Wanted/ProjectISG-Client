#pragma once

#include "CoreMinimal.h"
#include "LootContainerData.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"

#include "LootContainerSubsystem.generated.h"

DECLARE_DELEGATE_OneParam(FOnContainerCreated, FGuid);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API ULootContainerSubsystem : public UActorComponent, public IItemHandler
{
	GENERATED_BODY()

public:
	ULootContainerSubsystem();
	
	//TODO: 만약 데이터 저장 & 로드를 한다면 이것을 호출
	void LoadAllDataAsync();

	void CreateLootContainer(AActor* Causer, FGuid NewGuid, int32 Capacity);

	TArray<FItemMetaInfo> GetContainerItems(FGuid Guid);

	virtual FItemMetaInfo GetItemMetaInfo(FGuid Guid, const uint16 Index) override;
	
	virtual bool ChangeItem(AActor* Causer, FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index) override;

	virtual void SwapItem(AActor* Causer, FGuid Guid, const uint16 Prev, const uint16 Next) override;

	UFUNCTION(Server, Reliable)
	void Server_CreateLootContainer(FGuid NewGuid, uint16 Capacity);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CreateLootContainer(FGuid NewGuid, uint16 Capacity);

	UFUNCTION(Server, Reliable)
	void Server_SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next);

	UFUNCTION(Server, Reliable)
	void Server_ChangeItem(FGuid Guid, const FItemMetaInfo_Net& ItemInfo, const
	 uint16 Index);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ChangeItem(FGuid Guid, const FItemMetaInfo_Net& ItemInfo, const
	 uint16 Index);
	
protected:
	virtual void InitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_UpdateData();

private:
	UPROPERTY(ReplicatedUsing = OnRep_UpdateData);
	FLootContainerData Data;
};
