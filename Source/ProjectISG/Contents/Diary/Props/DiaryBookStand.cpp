#include "DiaryBookStand.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"


ADiaryBookStand::ADiaryBookStand()
{
}

void ADiaryBookStand::BeginPlay()
{
	Super::BeginPlay();
}

void ADiaryBookStand::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer);
	AMainPlayerController* PC = Cast<AMainPlayerController>(
		Player->GetController());

	PC->PushUI(EUIName::Popup_DiaryList);
}

FString ADiaryBookStand::GetDisplayText() const
{
	return IInteractionInterface::GetDisplayText();
}
