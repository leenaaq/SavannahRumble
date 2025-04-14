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

	void PrintScoreBoardLog();
public:
	UPROPERTY()
	TMap<FString, FPlayerScore> PlayerScoreBoard;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 GameOverScoreToWin = 3;
private:
	TArray<FString> LevelList;

public:
	FString FinalWinnerController = TEXT("NoOne");
};
