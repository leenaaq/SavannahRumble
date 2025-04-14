// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase_GameLevel.h"
#include "T6GMB_GL_FlagMap.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AT6GMB_GL_FlagMap : public AT6GameModeBase_GameLevel
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void SpreadPlayerControllers();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FlagModeLevelPlayingTime = 60;

	int32 RemainFlagModePlayingTime = 60;

protected:
	FTimerHandle FlagMapPlayingTimer;
	int32 SpawnIdx = 0;
protected:
	void OnFlagTimerEnd();
private:
	virtual void OnMainTimerElapsed() override;
};
