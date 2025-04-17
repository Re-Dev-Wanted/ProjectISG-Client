#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ProjectISG/Utils/MacroUtil.h"
#include "InventorySlotDragDropOperation.generated.h"

UCLASS()
class PROJECTISG_API UInventorySlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	GETTER_SETTER(TObjectPtr<UUserWidget>, OriginWidget)
	GETTER_SETTER(uint16, ItemIndex)

private:
	// 드래그 드롭을 위해 선택된 위젯 포인터를 저장해 이후 활용할 수 있도록 한다.
	UPROPERTY()
	TObjectPtr<UUserWidget> OriginWidget;

	uint16 ItemIndex;
};
