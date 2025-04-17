// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "T6GameModeBase_GameResult.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AT6GameModeBase_GameResult : public AT6GameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
private:
	FTimerHandle ResetGameTimerHandle;

	void OnGameResultTimerFinished();

	void ResetAndReturnToLobby();

private:
	UFUNCTION()
	void OnMainTimerElapsed();
public:
	FTimerHandle MainTimerHandle;
	FString Winner;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 RemainGameResultClosingTime = 60;
};
