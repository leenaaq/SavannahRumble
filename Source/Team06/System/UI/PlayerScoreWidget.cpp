// Fill out your copyright notice in the Description page of Project Settings.


#include "System/UI/PlayerScoreWidget.h"
#include "Components/TextBlock.h"

void UPlayerScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPlayerScoreWidget::SetPlayerInfo(const FString& Name, int32 Score)
{
    if (NameText) NameText->SetText(FText::FromString(Name));
    if (ScoreText) ScoreText->SetText(FText::AsNumber(Score));
    PlayerScore = Score;
}

int32 UPlayerScoreWidget::GetPlayerScore()
{
    return PlayerScore;
}
