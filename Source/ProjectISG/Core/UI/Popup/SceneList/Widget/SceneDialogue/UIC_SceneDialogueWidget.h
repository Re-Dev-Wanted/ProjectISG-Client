#pragma once

#include "CoreMinimal.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "UIC_SceneDialogueWidget.generated.h"

UCLASS()
class PROJECTISG_API UUIC_SceneDialogueWidget : public UBaseUIController
{
	GENERATED_BODY()

public:
	void StartSceneDialogue(const FString& SceneId, const uint8 SceneIndex);
};
