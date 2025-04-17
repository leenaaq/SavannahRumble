// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameStateBase_GameResult.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "Engine/World.h"
#include "System/GameSystem/FlagSpawnBox.h"
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
        SpawnPoints.Add(Start);
    }

    for (TActorIterator<AFlagSpawnBox> It(GetWorld()); It; ++It)
    {
        AFlagSpawnBox* WinnerSpawn = *It;
        WinnerSpawnPoint = WinnerSpawn;
        UE_LOG(LogTemp, Error, TEXT("WinnerSpawnRegistered"));
    }
}

