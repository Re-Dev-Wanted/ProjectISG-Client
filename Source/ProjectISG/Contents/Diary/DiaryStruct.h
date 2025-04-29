#pragma once

USTRUCT()
struct FGenerateDiaryRequest
{
	FString session_id;
	FString user_id;
	FString ingame_date;
};
