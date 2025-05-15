#pragma once

#include "CoreMinimal.h"
#include "LootContainerData.h"
#include "Components/ActorComponent.h"
#include "ProjectISG/Systems/Inventory/ItemData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"

#include "LootContainerSubsystem.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTISG_API ULootContainerSubsystem : public UActorComponent, public IItemHandler
{
	GENERATED_BODY()

public:
	ULootContainerSubsystem();
	
	//TODO: 만약 데이터 저장 & 로드를 한다면 이것을 호출
	void LoadAllDataAsync();

	FGuid CreateLootContainer(int32 Capacity);

	TArray<FItemMetaInfo> GetContainerItems(FGuid Guid);

	virtual FItemMetaInfo GetItemMetaInfo(FGuid Guid, const uint16 Index) override;
	
	virtual bool ChangeItem(FGuid Guid, const FItemMetaInfo& ItemInfo, const uint16 Index) override;

	virtual void SwapItem(FGuid Guid, const uint16 Prev, const uint16 Next) override;

protected:
	virtual void InitializeComponent() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	FLootContainerData Data;
};
