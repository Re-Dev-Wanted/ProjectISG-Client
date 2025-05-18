#pragma once

class ArrayUtil
{
public:
	template<typename T>
	static void KnuthShuffle(TArray<T>& Array)
	{
		const int32 Count = Array.Num();

		for (int32 i = Count - 1; i > 0; --i)
		{
			int32 j = FMath::RandRange(0, i); // 0 이상 i 이하의 랜덤 인덱스

			if (i != j)
			{
				Array.Swap(i, j);
			}
		}
	}

	/**
	 * 배열의 특정 위치에서 시작하여 최대 Count만큼의 요소를 추출한 새로운 TArray를 반환.
	 *
	 * @tparam T             배열 요소 타입 (템플릿)
	 * @param SourceArray    원본 배열
	 * @param StartIndex     추출 시작 인덱스 (0부터 시작)
	 * @param Count          최대 추출 개수 (SourceArray의 끝을 넘지 않음)
	 * @return               새로 생성된 부분 배열. 조건이 유효하지 않으면 빈 배열 반환.
	
	 */ 
	template<typename  T>
	static TArray<T> SubArrayRange(const TArray<T>& SourceArray, int32 StartIndex, int32 Count)
	{
		TArray<T> Result;

		if (StartIndex < 0 || StartIndex >= SourceArray.Num() || Count <= 0)
		{
			return Result;
		}

		int32 EndIndex = FMath::Min(StartIndex + Count, SourceArray.Num());

		for (int32 i = StartIndex; i < EndIndex; ++i)
		{
			Result.Add(SourceArray[i]);
		}

		return Result;
	}
};
