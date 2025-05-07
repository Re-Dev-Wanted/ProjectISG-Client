#include "DiaryComponent.h"

#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Diary/UI/DiaryEdit/UIC_DiaryEditUI.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

UDiaryComponent::UDiaryComponent()
{
}

void UDiaryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDiaryComponent::GenerateDiary()
{
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld()
		                                  , ATimeManager::StaticClass()));

	FGenerateDiaryRequest DiaryRequest;

	DiaryRequest.session_id = GetWorld()->GetGameState<AMainGameState>()->
	                                      GetSessionId();
	DiaryRequest.user_id = FSessionUtil::GetCurrentId(GetWorld());
	DiaryRequest.ingame_date = TimeManager->GetDateText();

	FApiRequest Request;
	Request.Path = TEXT("/diary/generate_diary");

	Request.Callback.BindLambda(
		[this](FHttpRequestPtr Req, FHttpResponsePtr Res, const bool IsSuccess)
		{
			const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
				GetOwner());

			AMainPlayerController* PC = Cast<AMainPlayerController>(
				Player->GetController());

			PC->PushUI(EUIName::Popup_DiaryEdit);

			FString Body = Res->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject; 
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(
				Body);

			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.
				IsValid())
			{
				FGenerateDiaryResponse ParsedData;

				if (FJsonObjectConverter::JsonObjectToUStruct(
					JsonObject.ToSharedRef()
					, FGenerateDiaryResponse::StaticStruct(), &ParsedData, 0
					, 0))
				{
					UUIC_DiaryEditUI* DiaryEditUIController = Cast<
						UUIC_DiaryEditUI>(
						PC->GetUIManageComponent()->ControllerInstances[
							EUIName::Popup_DiaryEdit]);

					DiaryEditUIController->InitializeDiaryToEdit(ParsedData);
				}
			}
		});

	FString DataParams;
	FJsonObjectConverter::UStructToJsonObjectString(DiaryRequest, DataParams);
	Request.Params = DataParams;

	FApiUtil::GetMainAPI()->PostApi(this, Request, GenerateDiaryResponse);
}
