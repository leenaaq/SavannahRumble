// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameStateBase_GameLevel.h"
#include "T6GSB_GL_FlagMap.generated.h"

class APlayerStart;
class AFlagSpawnBox;
class AController;
UCLASS()
class TEAM06_API AT6GSB_GL_FlagMap : public AT6GameStateBase_GameLevel
{
	GENERATED_BODY()
	
public:
	void InitSpawnPoint();
	UFUNCTION(Server,Reliable)
	void ServerChangeFlagOwner(AController* NewFlagOwner);
	void SpawnFlagActor();
	AController* GetFlagOwningController();
public:

	TArray<APlayerStart*> SpawnPoints;

	AFlagSpawnBox* FlagSpawnPoint;

protected:
	AController* FlagOwningController;
};
