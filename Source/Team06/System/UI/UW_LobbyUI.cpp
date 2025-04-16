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

	PigMeshChangeButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnPigMeshChangeButtonClicked);
	FoxMeshChangeButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnFoxMeshChangeButtonClicked);
	WolfMeshChangeButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnWolfMeshChangeButtonClicked);
	DeerMeshChangeButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnDeerMeshChangeButtonClicked);

	if (AddAIButton)
	{
		 AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			AddAIButton->SetIsEnabled(false);
			AddAIButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	DisableCharacterButtons();
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

void UUW_LobbyUI::OnPigMeshChangeButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerChangePSPlayerSkinName(FName("Pig"));
	}
}

void UUW_LobbyUI::OnFoxMeshChangeButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerChangePSPlayerSkinName(FName("Fox"));
	}
}

void UUW_LobbyUI::OnWolfMeshChangeButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerChangePSPlayerSkinName(FName("Wolf"));
	}
}

void UUW_LobbyUI::OnDeerMeshChangeButtonClicked()
{
	if (IsValid(OwnerPlayerController))
	{
		OwnerPlayerController->ServerChangePSPlayerSkinName(FName("Deer"));
	}
}

void UUW_LobbyUI::DisableCharacterButtons()
{
	if (PigMeshChangeButton)
	{
		AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			PigMeshChangeButton->SetIsEnabled(false);
			PigMeshChangeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (FoxMeshChangeButton)
	{
		AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			FoxMeshChangeButton->SetIsEnabled(false);
			FoxMeshChangeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (WolfMeshChangeButton)
	{
		AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			WolfMeshChangeButton->SetIsEnabled(false);
			WolfMeshChangeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (DeerMeshChangeButton)
	{
		AT6GameStateBase* GSB = Cast<AT6GameStateBase>(UGameplayStatics::GetGameState(this));
		if (!GSB || !GSB->bIsLobbyMode)
		{
			DeerMeshChangeButton->SetIsEnabled(false);
			DeerMeshChangeButton->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
