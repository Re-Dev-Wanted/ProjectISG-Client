#include "InputActionComponent.h"
#include "EnhancedInputComponent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Systems/Time/TimeManager.h"

UInputActionComponent::UInputActionComponent()
{
	bWantsInitializeComponent = true;
}


void UInputActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInputActionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(GetOwner());

	OwnerPlayer->OnInputBindingNotified.AddDynamic(
		this, &ThisClass::BindingInputActions);
}

void UInputActionComponent::BindingInputActions(
	UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(ToggleQuestInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OnToggleQuestUI);
	EnhancedInputComponent->BindAction(ToggleDiaryInputAction,
	                                   ETriggerEvent::Triggered, this,
	                                   &ThisClass::OnToggleDiaryUI);
}

void UInputActionComponent::OnToggleQuestUI()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());

	Cast<AMainPlayerController>(OwnerPlayer->GetController())->PushUI(
		EUIName::Popup_QuestListUI);
}

void UInputActionComponent::OnToggleDiaryUI()
{
	const AMainPlayerCharacter* OwnerPlayer = Cast<AMainPlayerCharacter>(
		GetOwner());
	const AMainPlayerState* PS = Cast<AMainPlayerState>(
		OwnerPlayer->GetPlayerState());
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		OwnerPlayer->GetController());

	if (!PS->GetTimeManager())
	{
		PC->Alert(EAlertType::Error, "알 수 없는 힘으로 가로막혀 있습니다.");
		return;
	}

	// 첫날 혹은 0일째에서는 일기 열람을 불가능하게 한다.
	if (PS->GetTimeManager()->GetDay() <= 1)
	{
		PC->Alert(EAlertType::Info, "열람할 일기 내용이 없습니다.");
		return;
	}

	Cast<AMainPlayerController>(OwnerPlayer->GetController())->PushUI(
		EUIName::Popup_DiaryList);
}
