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
};
