// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameStateBase_Lobby.h"
#include "Net/UnrealNetwork.h"


void AT6GameStateBase_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AlivePlayerContrReadyCountollerCount);
	DOREPLIFETIME(ThisClass, ReadyPlayerCount);
}

void AT6GameStateBase_Lobby::OnRep_ReadyCount()
{

}
