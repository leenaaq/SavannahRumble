// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameStateBase_GameResult.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "Engine/World.h"

void AT6GameStateBase_GameResult::InitGameResultSpawnPoint()
{
	if (!HasAuthority())
	{
		return;
	}

    // Find a PlayerStart with Tag == "Winner"
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        APlayerStart* Start = *It;
        if (Start->PlayerStartTag == "Winner")
        {
            WinnerSpawnPoint = Start;
        }
        else
        {
            SpawnPoints.Add(Start);
        }
    }
}

