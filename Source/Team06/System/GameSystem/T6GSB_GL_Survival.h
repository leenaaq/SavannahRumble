// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameStateBase_GameLevel.h"
#include "T6GSB_GL_Survival.generated.h"


UCLASS()
class TEAM06_API AT6GSB_GL_Survival : public AT6GameStateBase_GameLevel
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitiatePlayerLife(int32 Life);
	void InitSpawnPoint();
	UFUNCTION()
	void RespawnPlayer(AController* Player);
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 AliveSurviverCount = 0;
	
	UPROPERTY()
	TMap<TObjectPtr<AController>, int32> PlayerLives;

	TArray<APlayerStart*> SpawnPoints;

	TMap<AController*, APawn*> RespawningPawn;
};
