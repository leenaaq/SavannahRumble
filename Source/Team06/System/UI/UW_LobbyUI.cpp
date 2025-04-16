// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UI/UW_LobbyUI.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "Components/Button.h"
#include "System/GameSystem/T6GameModeBase_Lobby.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "Kismet/GameplayStatics.h"

UUW_LobbyUI::UUW_LobbyUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UUW_LobbyUI::Init(APCController_GamePlay* InOwner)
{
	OwnerPlayerController = InOwner;
}

void UUW_LobbyUI::UpdateReadyButtonState(bool bReady)
{
	if (!ReadyButton) return;

	const FLinearColor ColorToSet = bReady ? FLinearColor::Green : FLinearColor::White;

	// 배경 색상 바꾸기
	ReadyButton->SetColorAndOpacity(ColorToSet);
}

void UUW_LobbyUI::NativeConstruct()
{
	ReadyButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReadyButtonClicked);
	AddAIButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnAIAddButtonClicked);
	if (AddAIButton)
	{
		 AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			AddAIButton->SetIsEnabled(false);
			AddAIButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUW_LobbyUI::OnReadyButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerSetReady();
	}
}

void UUW_LobbyUI::OnAIAddButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerRequestAddingAI();
	}
}
