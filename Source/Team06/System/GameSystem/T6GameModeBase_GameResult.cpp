// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameModeBase_GameResult.h"
#include "System/GameSystem/T6GameInstance.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameSession.h"
#include "System/GameSystem/T6GameStateBase_GameResult.h"

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
    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);

    AT6GameStateBase_GameResult* GSB_GR = GetGameState<AT6GameStateBase_GameResult>();
    if (GSB_GR)
    {
        GSB_GR->InitGameResultSpawnPoint();

        SpreadPlayerbyGameResult();
    }
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

void AT6GameModeBase_GameResult::SpreadPlayerbyGameResult()
{
    AT6GameStateBase_GameResult* GSB_GR = GetGameState<AT6GameStateBase_GameResult>();

    if (!IsValid(GSB_GR))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid GSB_GR"));
        return;
    }
    else
    {
        UT6GameInstance* GI = GetGameInstance<UT6GameInstance>();
        if (!GI) return;
        if (GI->FinalWinnerController == "NoOne")
        {
            UE_LOG(LogTemp, Error, TEXT("No Winner Controller Error!!!!"));
            return;
        }

        int32 SpawnPointAmount = GSB_GR->SpawnPoints.Num();
        for (APCController_GamePlay* PCC : SessionPlayerControllers)
        {
            if (SpawnPointAmount == 0)
            {
                UE_LOG(LogTemp, Error, TEXT("SpawnPoints가 없습니다!"));
                return;
            }
            if (PCC->PlayerState)
            {
                if (GI->FinalWinnerController == PCC->PlayerState->GetPlayerName())
                {
                    UE_LOG(LogTemp, Error, TEXT("WinnerController %s Found!!!"), *PCC->PlayerState->GetPlayerName());
                    RestartPlayerAtPlayerStart(PCC, GSB_GR->WinnerSpawnPoint);
                }
                else
                {
                    RestartPlayerAtPlayerStart(PCC, GSB_GR->SpawnPoints[FMath::RandRange(0, SpawnPointAmount - 1)]);
                }
            }
        }
        if (GI->bIsAIAvailable)
        {
            //AI 있으면 등수 배치
        }
    }
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
