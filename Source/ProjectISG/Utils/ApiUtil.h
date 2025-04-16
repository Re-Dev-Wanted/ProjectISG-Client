#pragma once


#include "Interfaces/IHttpRequest.h"
#include "ApiUtil.generated.h"

USTRUCT()
struct FApiRequest
{
	GENERATED_BODY()

	// API에 Call하기 위한 Sub Path를 넣어주면 된다.
	// ex. /home (반드시 앞에 /를 붙이고 해야한다)
	UPROPERTY()
	FString Path;
	// Callback Function -> API가 성공, 실패 같은 Call이 완료되는 시점에서 실행되는
	// Lambda 함수를 넣어주면 된다.
	TFunction<void (FHttpRequestPtr Request, FHttpResponsePtr Response,
	                const bool IsSuccess)> Callback;
	// API Call이 있는 경우에 Post 같은 객체로 전달해야 하는 요청이 있을 시 설정하는 값으로,
	// Get에서는 어차피 값을 넣어주지 않아도 되니 Get은 무시해도 된다.
	// (최신 Get 문법으로 진행하지 않을 것이라 Params 안씀)
	UPROPERTY()
	FString Params;
	// Header에 들어가는 부가적인 정보
	// Content-Type인 경우 어차피 SetHeader로 처리해야 하기 때문에
	// 중복되어도 중간에 값이 바뀐다고 인지하면 된다.
	UPROPERTY()
	TMap<FString, FString> Header;
};

USTRUCT()
struct FApiResponse
{
	GENERATED_BODY()

	// 해당 API가 현재 실행 중 인지를 알기 위한 값이다.
	UPROPERTY()
	bool IsLoading;
};

class FApiUtil
{
public:
	static FApiUtil* GetMainAPI()
	{
		if (!MainAPI)
		{
			MainAPI = new FApiUtil();
			MainAPI->URL = "http://localhost:3000";
		}

		return MainAPI;
	}

	void GetApi(UObject* Caller, const FApiRequest& Request,
	            FApiResponse& Response) const;
	void PostApi(UObject* Caller, const FApiRequest& Request,
	             FApiResponse& Response) const;

private:
	static FApiUtil* MainAPI;

	FString URL;
};
