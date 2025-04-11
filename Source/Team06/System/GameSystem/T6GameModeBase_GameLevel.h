// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "T6GameModeBase_GameLevel.generated.h"

DECLARE_DELEGATE_OneParam(FGameWinCallback, APlayerController*);

class APlayerController;

UCLASS()
class TEAM06_API AT6GameModeBase_GameLevel : public AT6GameModeBase
{
	GENERATED_BODY()
protected:
	void NotifyToAllPlayer(const FString& NotificationString) override;
public:
	UFUNCTION(BlueprintCallable)
	void HandlePlayerGameWin(APlayerController* Winner);

	UFUNCTION(BlueprintCallable)
	void HandleRandomPlayerGameWin();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RespawnWaitingTime = 10;

	int32 RemainRespawnWaitingTimeForPlaying = 10;

	int32 MinimumPlayerCountForPlaying = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GameLevelWaitingTime = 10;

	int32 RemainGameLevelWaitingTimeForPlaying = 10;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnMainTimerElapsed();
public:
	FTimerHandle MainTimerHandle;
	FGameWinCallback OnGameWinHandled;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GameStartCountDown = 5;

	int32 RemainGameStartTimeForEnding = 5;


#pragma region Ready

public:
	void SetPlayerReady(APCController_GamePlay* PlayerController, bool bIsReady);
	bool IsMajorityReady() const;

protected:
	TMap<TObjectPtr<APCController_GamePlay>, bool> GameLevelPlayerReady;
#pragma endregion

#pragma region GameEnd

public:
	bool bIsGameLevelFinished = false;
protected:
	void InitiatedGameLevelEnding(APlayerController* Winner);
#pragma endregion

};
