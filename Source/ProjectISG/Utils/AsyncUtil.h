#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

class AsyncUtil
{
public:
	template <typename T>
	static void LoadAsync(TSoftObjectPtr<T> SoftObjectPtr, TFunction<void(T*)> OnResultCallback)
	{
		if (SoftObjectPtr.IsNull())
		{
			if (OnResultCallback)
			{
				OnResultCallback(nullptr);
			}
			return;
		}

		FSoftObjectPath AssetPath = SoftObjectPtr.ToSoftObjectPath();
		LoadAsyncInternal<T>(AssetPath, OnResultCallback);
	}

	template <typename T>
	static void LoadAsync(TSoftObjectPtr<T> SoftObjectPtr, T*& OutObject)
	{
		if (SoftObjectPtr.IsNull())
		{
			OutObject = nullptr;
			return;
		}

		FSoftObjectPath AssetPath = SoftObjectPtr.ToSoftObjectPath();

		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		Streamable.RequestAsyncLoad
		(
			AssetPath,
			[AssetPath, OutObject]()
			{
				UObject* LoadedObject = AssetPath.ResolveObject();
				if (!LoadedObject)
				{
					LoadedObject = AssetPath.TryLoad();
				}

				OutObject = Cast<T>(LoadedObject);
			}
		);
	}

private:
	template <typename T>
	static void LoadAsyncInternal(FSoftObjectPath AssetPath, TFunction<void(T*)> OnResultCallback)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		Streamable.RequestAsyncLoad
		(
			AssetPath,
			[AssetPath, OnResultCallback]()
			{
				UObject* LoadedObject = AssetPath.ResolveObject();
				if (!LoadedObject)
				{
					LoadedObject = AssetPath.TryLoad();
				}

				T* Casted = Cast<T>(LoadedObject);

				if (OnResultCallback)
				{
					OnResultCallback(Casted);
				}
			}
		);
	}
};
