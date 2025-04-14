// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameModeBase_GameResult.h"
#include "System/GameSystem/T6GameInstance.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameSession.h"

AActor* AT6GameModeBase_GameResult::ChoosePlayerStart_Implementation(AController* Player)
{
    UT6GameInstance* GI = GetGameInstance<UT6GameInstance>();
    if (!GI)
    {
        UE_LOG(LogTemp, Error, TEXT("GameInstance not valid."));
        return Super::ChoosePlayerStart_Implementation(Player);
    }

    FString WinnerName = GI->FinalWinnerController;
    if (WinnerName == "NoOne")
    {
        UE_LOG(LogTemp, Error, TEXT("FinalWinnerController not set! Cannot spawn properly."));
        return Super::ChoosePlayerStart_Implementation(Player);
    }

    APlayerState* PS = Player->PlayerState;
    if (PS && PS->GetPlayerName() == WinnerName)
    {
        // Find a PlayerStart with Tag == "Winner"
        for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
        {
            APlayerStart* Start = *It;
            if (Start->PlayerStartTag == "Winner")
            {
                UE_LOG(LogTemp, Log, TEXT("Spawning winner %s at WinnerStart"), *WinnerName);
                return Start;
            }
        }
    }

    // Default fallback
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AT6GameModeBase_GameResult::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (UT6GameInstance* GI = GetGameInstance<UT6GameInstance>())
        {
            UE_LOG(LogTemp, Warning, TEXT("Final winner is: %s"), *GI->FinalWinnerController);
        }
    }
    //GetWorld()->GetTimerManager().SetTimer(ResetGameTimerHandle, this, &AT6GameModeBase_GameResult::OnGameResultTimerFinished, 60.f, false);
    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);
}

void AT6GameModeBase_GameResult::OnGameResultTimerFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("GameResult: Time expired. Resetting the game and moving to Lobby."));
    ResetAndReturnToLobby();
}

void AT6GameModeBase_GameResult::ResetAndReturnToLobby()
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        APlayerController* PC = Iterator->Get();
        if (PC && PC->GetNetConnection())
        {
            UE_LOG(LogTemp, Log, TEXT("Kicking player %s"), *PC->GetPlayerState<APlayerState>()->GetPlayerName());
            GameSession->KickPlayer(PC, FText::FromString(TEXT("Game has Ended")));
        }
    }

    // GameInstance 초기화
    if (UT6GameInstance* GI = GetGameInstance<UT6GameInstance>())
    {
        GI->PlayerScoreBoard.Empty();
        GI->FinalWinnerController = TEXT("NoOne");
    }

    // Lobby 레벨로 이동
    const FString LobbyMapPath = TEXT("/Game/Team6/GameSystem/GS_Level/TestLobby?listen");
    UGameplayStatics::OpenLevel(this, FName(*LobbyMapPath), true);
}

void AT6GameModeBase_GameResult::OnMainTimerElapsed()
{
    --RemainGameResultClosingTime;
    FString NotificationString = FString::Printf(TEXT(""));
    NotificationString = FString::Printf(TEXT("Wait %d seconds for ServerClosing."), RemainGameResultClosingTime);
    NotifyToAllPlayer(NotificationString);

    if (RemainGameResultClosingTime <=0)
    {
        MainTimerHandle.Invalidate();
        
        OnGameResultTimerFinished();
    }
}
