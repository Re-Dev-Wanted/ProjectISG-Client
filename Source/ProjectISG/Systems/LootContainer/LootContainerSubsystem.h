#pragma once

#include "CoreMinimal.h"
#include "LootContainerData.h"
#include "ProjectISG/Systems/Inventory/ItemHandler.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LootContainerSubsystem.generated.h"

UCLASS()
class PROJECTISG_API ULootContainerSubsystem : public UGameInstanceSubsystem,
 public IItemHandler
{
	GENERATED_BODY()

public:
	//TODO: 만약 데이터 저장 & 로드를 한다면 이것을 호출
	void LoadAllDataAsync();

	FGuid CreateLootContainer();

	virtual bool ChangeItem(const FItemMetaInfo& ItemInfo, const uint16 Index) override;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	FLootContainerData Data;
};
