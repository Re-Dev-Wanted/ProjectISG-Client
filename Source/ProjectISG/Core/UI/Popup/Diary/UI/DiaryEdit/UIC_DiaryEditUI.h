#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_DiaryEditUI.generated.h"

struct FGenerateDiaryResponse;

UCLASS()
class PROJECTISG_API UUIC_DiaryEditUI : public UBaseUIController
{
	GENERATED_BODY()

public:
	void InitializeDiaryToEdit(const FGenerateDiaryResponse& Diary);

protected:
	virtual void AppearUI(const EUILayer Layer) override;

private:
	UFUNCTION()
	void OnClickToSaveDiary();
};
