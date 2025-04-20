#include "PlacementGridContainer.h"

// Unreal에서 FastArraySerializer의 TStructOpsTypeTraits 특수화는 컴파일 캐시와 UHT(UHT-generated headers) 시스템이 미묘하게 엮여 있음
// 최초 한 번만 빌드 성공하고 이후엔 C2908/C2099 등 에러가 반복
// header에서 선언하지 말자 : TStructOpsTypeTraits 특수화 중복 정의 에러(C2908, LNK2005 등) 발생
// TStructOpsTypeTraits<> 특수화는 매크로 기반으로 UHT가 처리하지 않기 때문에, 헤더에 있으면 Unreal Build Tool(UHT/UHT-generated headers)과 충돌 가능성 있음

template<>
struct TStructOpsTypeTraits<FPlacementGridContainer> : TStructOpsTypeTraitsBase2<FPlacementGridContainer>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

// 정적 객체로 등록해 컴파일때 실행되도록
struct FPlacementGridContainerTraitsRegistrar
{
	FPlacementGridContainerTraitsRegistrar()
	{
		FPlacementGridContainer::StaticStruct();
	}
};

static FPlacementGridContainerTraitsRegistrar RegisterTraits;