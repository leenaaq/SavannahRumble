// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameModeBase_GameLevel.h"
#include "T6GMB_GL_Survival.generated.h"

class APlayerStart;
UCLASS()
class TEAM06_API AT6GMB_GL_Survival : public AT6GameModeBase_GameLevel
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	void OnCharacterDead(AController* InController);
	void RegistPCSessionToAlive();
	void HandleDeathController(AController* InController);
	void EndingSurvivalGameLevel();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AController>> AlivePlayerControllers;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<AController>> DeadPlayerControllers;


private:
	virtual void OnMainTimerElapsed() override;


protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 SurvivalGamePlayerLife = 1;
	bool bRegistered = false;
};
