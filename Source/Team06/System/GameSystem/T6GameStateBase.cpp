// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GameStateBase.h"
#include "Net/UnrealNetwork.h"

void AT6GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass,MatchState);
}
