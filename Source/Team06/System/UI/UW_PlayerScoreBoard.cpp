// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UI/UW_PlayerScoreBoard.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "System/UI/PlayerScoreWidget.h"
#include "System/GameSystem/T6GameInstance.h"

void UUW_PlayerScoreBoard::InitializeScoreBoard(const TArray<FPlayerScoreEntry>& PlayerScores)
{
    if (!PlayerContainer || !PlayerScoreEntryClass) return;

    ClearBoard();

    // 1. 정렬을 위한 배열 복사
    TArray<FPlayerScoreEntry> SortedArray = PlayerScores;

    // 2. 내림차순 정렬
    SortedArray.Sort([](const FPlayerScoreEntry& A, const FPlayerScoreEntry& B)
        {
            if (A.Score == B.Score)
            {
                return A.PlayerName.Compare(B.PlayerName) <0;
            }
            return A.Score > B.Score;
        });

    // 3. 정렬된 순서대로 위젯 생성
    for (const auto& Entry : SortedArray)
    {
        UPlayerScoreWidget* Widget = CreateWidget<UPlayerScoreWidget>(this, PlayerScoreEntryClass);
        Widget->SetPlayerInfo(Entry.PlayerName, Entry.Score);
        PlayerContainer->AddChildToVerticalBox(Widget);
        ScoreEntries.Add(Entry.PlayerName, Widget);
    }
}

void UUW_PlayerScoreBoard::ClearBoard()
{
    if (PlayerContainer)
    {
        PlayerContainer->ClearChildren();
        ScoreEntries.Empty();
    }
}

void UUW_PlayerScoreBoard::NativeConstruct()
{
	Super::NativeConstruct();
}
