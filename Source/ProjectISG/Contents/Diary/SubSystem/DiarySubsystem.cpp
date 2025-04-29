#include "DiarySubsystem.h"

#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectISG/Contents/Diary/DiaryStruct.h"
#include "ProjectISG/Core/GameMode/MainGameMode.h"
#include "ProjectISG/Systems/Time/TimeManager.h"
#include "ProjectISG/Utils/ApiUtil.h"
#include "ProjectISG/Utils/SessionUtil.h"

void UDiarySubsystem::GenerateDiary(const FApiRequestCallback& Callback,
                                    FApiResponse& Response)
{
	const ATimeManager* TimeManager = Cast<ATimeManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(),
		                                  ATimeManager::StaticClass()));

	FGenerateDiaryRequest DiaryRequest;

	DiaryRequest.session_id = GetWorld()->GetAuthGameMode<AMainGameMode>()->
	                                      GetSessionId();
	DiaryRequest.user_id = FSessionUtil::GetCurrentId(GetWorld());
	DiaryRequest.ingame_date = FString::FromInt(
		TimeManager->GetTotalPlayingDay());

	FApiRequest Request;
	Request.Path = TEXT("/log/generate_diary");
	Request.Callback = Callback.Callback;

	FJsonObjectConverter::UStructToJsonObjectString(
		DiaryRequest, Request.Params);

	FApiUtil::GetMainAPI()->PostApi(this, Request, Response);
}
