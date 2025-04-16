// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"


void AT6GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,MatchState);
	DOREPLIFETIME(ThisClass, bIsLobbyMode);
}

void AT6GameStateBase::InitAISpawnPoint()
{   
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        AISpawnPoints.Add(*It); // *It으로 액터 포인터 추가
    }

}
