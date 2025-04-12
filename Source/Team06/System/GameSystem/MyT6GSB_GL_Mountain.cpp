// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/MyT6GSB_GL_Mountain.h"
#include "System/GameSystem/CheckpointBox.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "EngineUtils.h"

void AMyT6GSB_GL_Mountain::BeginPlay()
{
    Super::BeginPlay();
    bReplicates = true;
    InitiateCheckpointBoxes();
}

void AMyT6GSB_GL_Mountain::AddPlayerToArray(APlayerController* Player)
{
    if (Player && !PlayerHighestCheckpointIndex.Contains(Player))
    {
        PlayerHighestCheckpointIndex.Add(Player, 0); // 기본값은 0번 체크포인트
    }
}

void AMyT6GSB_GL_Mountain::UpdatePlayerCheckpoint(APlayerController* PC, int32 NewIndex)
{
    if (!PC) return;

    int32* CurrentIndexPtr = PlayerHighestCheckpointIndex.Find(PC);
    if (!CurrentIndexPtr)
    {
        AddPlayerToArray(PC);
        CurrentIndexPtr = PlayerHighestCheckpointIndex.Find(PC);
    }

    if (CurrentIndexPtr && NewIndex > *CurrentIndexPtr)
    {
        *CurrentIndexPtr = NewIndex;
        UE_LOG(LogTemp, Log, TEXT("Player %s updated checkpoint index to %d"), *PC->GetName(), NewIndex);
    }
}

FVector AMyT6GSB_GL_Mountain::GetCheckpointLocationForPlayer(APlayerController* PC)
{
    int32* FoundIndex = PlayerHighestCheckpointIndex.Find(PC);
    if (!FoundIndex) return FVector::ZeroVector;

    for (ACheckpointBox* Checkpoint : AllCheckpoints)
    {
        if (Checkpoint && Checkpoint->CheckPointIndex == *FoundIndex)
        {
            return Checkpoint->CheckPointLocation;
        }
    }
    return FVector::ZeroVector;
}

void AMyT6GSB_GL_Mountain::InitiateCheckpointBoxes()
{
    if (HasAuthority())
    {
        for (TActorIterator<ACheckpointBox> It(GetWorld()); It; ++It)
        {
            AllCheckpoints.Add(*It);
        }

        AllCheckpoints.Sort([](const ACheckpointBox& A, const ACheckpointBox& B) {
            return A.CheckPointIndex < B.CheckPointIndex;
            });

        UE_LOG(LogTemp, Log, TEXT("Checkbox has updated by GSB"));
    }
}
