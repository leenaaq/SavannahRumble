// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "T6GameStateBase_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AT6GameStateBase_Lobby : public AT6GameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AlivePlayerContrReadyCountollerCount = 0;
	UPROPERTY(ReplicatedUsing = OnRep_ReadyCount)
	int32 ReadyPlayerCount = 0;

	UFUNCTION()
	void OnRep_ReadyCount(); 

};
