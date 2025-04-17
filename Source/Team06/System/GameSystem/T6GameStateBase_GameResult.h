// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameStateBase.h"
#include "T6GameStateBase_GameResult.generated.h"

class APlayerStart;
class AFlagSpawnBox;
UCLASS()
class TEAM06_API AT6GameStateBase_GameResult : public AT6GameStateBase
{
	GENERATED_BODY()
	
public:
	void InitGameResultSpawnPoint();

public:
	TArray<TObjectPtr<APlayerStart>> SpawnPoints;
	
	TObjectPtr<AFlagSpawnBox> WinnerSpawnPoint;
};
