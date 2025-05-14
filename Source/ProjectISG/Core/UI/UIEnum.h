#pragma once

#include "UIEnum.generated.h"

UENUM()
enum class EUILayer: uint8
{
	// 체력바, 핫슬롯, 미니 맵 등의 기본 HUD
	// -> 게임 플레이 중 항상 켜져있어야 함.
	// 즉 Gameplay는 웬만해서는 MainHUD에만 해당된다.
	Gameplay,
	// 장비 창, 설정 창 등이 이에 해당된다.
	// 웬만한 추가 UI는 이에 해당됨
	Popup,
	// 알림, 경고, 튜토리얼 등의 부가적인 정보 Modal이 이에 해당됨
	Modal,
	// 로딩 UI, 커서 잠금을 위한 UI Layer로
	// 가장 최상위에 올라가는 UI가 이에 해당된다.
	Loading,
};

UENUM()
enum class EUIName: uint32
{
	Gameplay_MainHUD, Popup_InventoryUI, Popup_CookingRecipeUI,
	Popup_CookingQTE, Popup_TradingUI, Modal_BuyNotification,
	Modal_SellNotification, Popup_DiaryEdit, Popup_DiaryList,
	Modal_ExitInteractUI, Modal_FishingUI, Popup_SceneListUI,
	Popup_MainLobby, Popup_MultiPlay,
};
