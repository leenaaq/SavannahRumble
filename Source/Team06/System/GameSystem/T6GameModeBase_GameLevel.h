// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "T6GameModeBase_GameLevel.generated.h"

DECLARE_DELEGATE_OneParam(FGameWinCallback, AController*);

class APlayerController;

UCLASS()
class TEAM06_API AT6GameModeBase_GameLevel : public AT6GameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void HandlePlayerGameWin(AController* Winner);

	UFUNCTION(BlueprintCallable)
	void HandleRandomPlayerGameWin();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RespawnWaitingTime = 5;

	int32 RemainRespawnWaitingTimeForPlaying = 5;

	int32 MinimumPlayerCountForPlaying = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GameLevelWaitingTime = 10;

	int32 RemainGameLevelWaitingTimeForPlaying = 10;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
private:
	UFUNCTION()
	virtual void OnMainTimerElapsed();
public:
	FTimerHandle MainTimerHandle;
	FGameWinCallback OnGameWinHandled;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GameStartCountDown = 10;

	int32 RemainGameStartTimeForEnding = 10;


#pragma region Ready

public:
	virtual void SetPlayerReady(APCController_GamePlay* PlayerController, bool bIsReady) override;
	bool IsMajorityReady() const;

protected:
	TMap<TObjectPtr<APCController_GamePlay>, bool> GameLevelPlayerReady;
#pragma endregion

#pragma region GameEnd

public:
	bool bIsGameLevelFinished = false;

protected:
	void InitiatedGameLevelEnding(AController* Winner);

#pragma endregion
	void UpdateAllClientScoreBoards();
};
