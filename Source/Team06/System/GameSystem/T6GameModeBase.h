// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6EMatchState.h"
#include "GameFramework/GameModeBase.h"
#include "T6GameModeBase.generated.h"


class APCController_GamePlay;
UCLASS()
class TEAM06_API AT6GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void NotifyToAllPlayer(const FString& NotificationString);
public:
	virtual APlayerController* Login(UPlayer* NewPlayer,
		ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<APCController_GamePlay>> SessionPlayerControllers;
#pragma region Level

public:
	void ChangeGameLevel();
	void OnShowingLoadingScreen();
#pragma endregion

};
