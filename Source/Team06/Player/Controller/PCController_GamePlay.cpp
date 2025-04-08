// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/PCController_GamePlay.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "System/UI/UW_LobbyUI.h"
#include "Components/TextBlock.h"
#include "System/GameSystem/T6GameModeBase_Lobby.h"

void APCController_GamePlay::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);

	DOREPLIFETIME(ThisClass, bIsReady);
}

void APCController_GamePlay::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	bShowMouseCursor = false;
	if (IsLocalController() == false)
	{
		return;
	}

	if (IsValid(NotificationTextUIClass) == true)
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
		if (IsValid(NotificationTextUI) == true)
		{
			NotificationTextUI->AddToViewport(1);

			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (LobbyWidgetClass)
	{
		LobbyWidget = CreateWidget<UUW_LobbyUI>(this, LobbyWidgetClass);
		if (LobbyWidget)
		{
			LobbyWidget->AddToViewport(3);
			// 위젯에 자신 전달
			LobbyWidget->Init(this);
			LobbyWidget->UpdateReadyButtonState(bIsReady);
			LobbyWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void APCController_GamePlay::ShowGameLoadingWidget_Implementation()
{
	if (HasAuthority() == false)
	{
		if (IsValid(GameLoadingClass) == true)
		{
			UUserWidget* GameResultUI = CreateWidget<UUserWidget>(this, GameLoadingClass);
			if (IsValid(GameResultUI) == true)
			{
				GameResultUI->AddToViewport(4);

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(GameResultUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = false;
			}
		}
	}
}

void APCController_GamePlay::OnHandleLobbyUI()
{
	const bool bIsNowVisible = LobbyWidget->IsVisible();

	if (bIsNowVisible)
	{
		// 숨기기
		LobbyWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		// 보이기
		LobbyWidget->SetVisibility(ESlateVisibility::Visible);
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}

void APCController_GamePlay::ServerSetReady_Implementation()
{
	this->bIsReady = !this->bIsReady;

	AT6GameModeBase_Lobby* LobbyGM = GetWorld()->GetAuthGameMode<AT6GameModeBase_Lobby>();
	if (IsValid(LobbyGM))
	{
		LobbyGM->SetPlayerReady(this, bIsReady);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("UnExpected ReadyButton Occured by PCController_GamePlay"));
	}
}

void APCController_GamePlay::OnRep_bIsReady()
{
	if (IsLocalController() && IsValid(LobbyWidget))
	{
		LobbyWidget->UpdateReadyButtonState(bIsReady);
	}
}
