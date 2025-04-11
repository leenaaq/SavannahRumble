// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase.h"
#include "T6GameModeBase_Lobby.generated.h"

class APCController_GamePlay;

UCLASS()
class TEAM06_API AT6GameModeBase_Lobby : public AT6GameModeBase
{
	GENERATED_BODY()
protected:
	void NotifyToAllPlayer(const FString& NotificationString) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LobbyWaitingTime = 15;

	int32 RemainLobbyWaitingTimeForPlaying = 15;

	int32 MinimumPlayerCountForPlaying = 2;
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnMainTimerElapsed();
public:
	FTimerHandle MainTimerHandle;
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 LobbyEndingTime = 15;

	int32 RemainWaitingTimeForEnding = 15;


#pragma region Ready

public:
	virtual void SetPlayerReady(APCController_GamePlay* PlayerController, bool bIsReady) override;
	bool IsMajorityReady() const;
	// bool IsAllReady() const;
protected:
	TMap<TObjectPtr<APCController_GamePlay>, bool> LobbyPlayerReady;
#pragma endregion
};
