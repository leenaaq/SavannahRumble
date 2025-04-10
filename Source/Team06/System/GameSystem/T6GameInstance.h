// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "T6GameInstance.generated.h"

USTRUCT()
struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 TotalWins = 0;
};

UCLASS()
class TEAM06_API UT6GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	FString GetRandomLevel();
	void LoadLevelsFromFolder();

	void CheckWinScore();
	void RegisterPlayer(const FString& PlayerName);
	void AddWinForPlayer(const FString& PlayerName);
public:
	UPROPERTY()
	TMap<FString, FPlayerScore> PlayerScoreBoard;
private:
	TArray<FString> LevelList;
	int32 GameOverScoreToWin = 3;
};
