// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameModeBase_GameLevel.h"
#include "System/GameSystem/T6GameInstance.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "player/PlayerState/PlayerCharacterState.h"

void AT6GameModeBase_GameLevel::HandlePlayerGameWin(APlayerController* Winner)
{
    if (!Winner || !Winner->PlayerState) return;

    FString Name = Winner->PlayerState->GetPlayerName();

    if (UT6GameInstance* GI = GetGameInstance<UT6GameInstance>())
    {
        GI->AddWinForPlayer(Name);
        UE_LOG(LogTemp, Log, TEXT("%s now has +1 win"), *Name);
    }
}
