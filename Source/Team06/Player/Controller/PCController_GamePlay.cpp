// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Controller/PCController_GamePlay.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "System/UI/UW_LobbyUI.h"
#include "Components/TextBlock.h"
#include "System/GameSystem/T6GameModeBase_Lobby.h"
#include "System/GameSystem/T6GameModeBase_GameLevel.h"
#include "System/GameSystem/MyT6GSB_GL_Mountain.h"
#include "Player/Player/PlayerCharacter.h"
#include "System/GameSystem/T6GMB_GL_Survival.h"
#include "Kismet/GameplayStatics.h"
#include "System/GameSystem/T6GameInstance.h"


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

	AT6GameModeBase* GM = GetWorld()->GetAuthGameMode<AT6GameModeBase>();
	if (IsValid(GM))
	{
		GM->SetPlayerReady(this, bIsReady);
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

void APCController_GamePlay::Server_TriggerRandomPlayerWin_Implementation()
{
	AT6GameModeBase_GameLevel* GM = GetWorld() ? Cast<AT6GameModeBase_GameLevel>(GetWorld()->GetAuthGameMode()) : nullptr;
	if (GM)
	{
		GM->HandleRandomPlayerGameWin();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong GameModeBase Called in PCController_GamePlay"));
	}
}
void APCController_GamePlay::ServerNotifyGoalReached_Implementation()
{
	AT6GameModeBase_GameLevel* GM = GetWorld()->GetAuthGameMode<AT6GameModeBase_GameLevel>();
	if (GM)
	{
		GM->HandlePlayerGameWin(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong GameModeBase Called in PCController_GamePlay"));
	}
}

void APCController_GamePlay::ServerTeleportToCheckpoint_Implementation()
{
	AMyT6GSB_GL_Mountain* GSM = GetWorld()->GetGameState<AMyT6GSB_GL_Mountain>();

	ACharacter* MyChar = GetCharacter();


	if (GSM && MyChar)
	{
		FVector TargetLoc = GSM->GetCheckpointLocationForPlayer(this);
		MyChar->SetActorLocation(TargetLoc);
	}
}

void APCController_GamePlay::OnPlayerFalltoDeath_Implementation()
{
	AT6GMB_GL_Survival* GMB_Survival = Cast<AT6GMB_GL_Survival>(UGameplayStatics::GetGameMode(this));

	if (HasAuthority() == true && IsValid(GMB_Survival) == true)
	{
		GMB_Survival->OnCharacterDead(this);
	}
}

bool APCController_GamePlay::ServerRequestAddingAI_Validate()
{
	if (!HasAuthority())
	{
		return false;
	}

	UT6GameInstance* GI = GetGameInstance<UT6GameInstance>();

	if (GI)
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Add AI"));
		return false;
	}
}

void APCController_GamePlay::ServerRequestAddingAI_Implementation()
{
	UT6GameInstance* GI = GetGameInstance<UT6GameInstance>();
	GI->bIsAIAvailable = true;
	if (GI->bIsAISpawned)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI already Spawned"));
		return;
	}
	else
	{
		GI->bIsAISpawned = true;
		// AI 추가
		GI->AddAIPlayer();
		// GI에서 AI 점수판 추가
		AT6GameModeBase* GMB = GetWorld()->GetAuthGameMode<AT6GameModeBase>();
		if (GMB)
		{
			// GameMode에서 점수판의 이름을 가진 AI 추가
			GMB->SpawnAIControllers();
		}


	}
}