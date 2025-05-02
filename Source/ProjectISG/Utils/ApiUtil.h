#pragma once

#include "Interfaces/IHttpRequest.h"
#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "ApiUtil.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnApiCallbackNotified, FHttpRequestPtr Request
							, FHttpResponsePtr Response, const bool IsSuccess);

USTRUCT()
struct FApiRequestCallback
{
	GENERATED_BODY()

	FOnApiCallbackNotified Callback;
};

USTRUCT()
struct FApiRequest : public FApiRequestCallback
{
	GENERATED_BODY()

	UPROPERTY()
	FString Path;

	UPROPERTY()
	FString Params;

	UPROPERTY()
	TMap<FString, FString> Header;
};

USTRUCT()
struct FApiResponse
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsLoading = false;

	UPROPERTY()
	int32 RequestId = -1;
};

class FApiUtil
{
public:
	static FApiUtil* GetMainAPI();

	template <typename StructType>
	static bool DeserializeJsonArray(const FString& JsonString
									, TArray<StructType>& OutArray)
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(
			JsonString);
		TArray<TSharedPtr<FJsonValue>> JsonArray;

		if (!FJsonSerializer::Deserialize(Reader, JsonArray))
		{
			return false;
		}

		for (const auto& Value : JsonArray)
		{
			StructType Item;
			if (!FJsonObjectConverter::JsonObjectToUStruct(
				Value->AsObject().ToSharedRef(), StructType::StaticStruct()
				, &Item))
			{
				continue;
			}
			OutArray.Add(Item);
		}

		return true;
	}

	template <typename StructType>
	static bool DeserializeJsonObject(const FString& JsonString
									, StructType& OutObject)
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(
			JsonString);
		TSharedPtr<FJsonObject> JsonObject;

		if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.
			IsValid())
		{
			return false;
		}

		return FJsonObjectConverter::JsonObjectToUStruct(
			JsonObject.ToSharedRef(), StructType::StaticStruct(), &OutObject);
	}

	void GetApi(UObject* Caller, const FApiRequest& Request
				, FApiResponse& Response) const;
	void PostApi(UObject* Caller, const FApiRequest& Request
				, FApiResponse& Response) const;

private:
	// 한번 할당된 포인터인 경우 다른 값으로 교체하는 것이 불가능해짐을 의미함.
	static TUniquePtr<FApiUtil> MainAPI;
	FString URL;

	void SendRequest(UObject* Caller, const FString& Verb
					, const FApiRequest& Request, FApiResponse& Response) const;
};
