// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlayerScoreBoard.generated.h"

class UPlayerScoreWidget;
class UVerticalBox;
struct FPlayerScoreEntry;

UCLASS()
class TEAM06_API UUW_PlayerScoreBoard : public UUserWidget
{
	GENERATED_BODY()
public:
    void InitializeScoreBoard(const TArray<FPlayerScoreEntry>& PlayerScores);
    void ClearBoard();

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerContainer;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UPlayerScoreWidget> PlayerScoreEntryClass;

private:
    TMap<FString, UPlayerScoreWidget*> ScoreEntries;
};
