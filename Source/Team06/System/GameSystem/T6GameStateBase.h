// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "T6GameStateBase.generated.h"

class APlayerStart;

UCLASS()
class TEAM06_API AT6GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitAISpawnPoint();
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	EMatchState MatchState = EMatchState::Waiting;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsLobbyMode = false;

public:

	TArray<APlayerStart*> AISpawnPoints;
};
