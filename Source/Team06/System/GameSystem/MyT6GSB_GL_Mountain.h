// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameSystem/T6GameStateBase_GameLevel.h"
#include "MyT6GSB_GL_Mountain.generated.h"

class ACheckpointBox;
class APlayerController;
UCLASS()
class TEAM06_API AMyT6GSB_GL_Mountain : public AT6GameStateBase_GameLevel
{
	GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<APlayerController*, int32> PlayerHighestCheckpointIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<ACheckpointBox*> AllCheckpoints;

public:
    virtual void BeginPlay() override;
    void AddPlayerToArray(APlayerController* Player);
    void UpdatePlayerCheckpoint(APlayerController* PC, int32 NewIndex);
    FVector GetCheckpointLocationForPlayer(APlayerController* PC);

    void InitiateCheckpointBoxes();
};
