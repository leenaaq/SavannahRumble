// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UPlayerScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    void SetPlayerInfo(const FString& Name, int32 Score);
    int32 GetPlayerScore();
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* NameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ScoreText;

    int32 PlayerScore;
};
