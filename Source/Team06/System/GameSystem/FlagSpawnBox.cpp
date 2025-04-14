// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/FlagSpawnBox.h"
#include "Components/BoxComponent.h"
#include "Player/Component/FlagActor.h"
#include "Engine/World.h"

AFlagSpawnBox::AFlagSpawnBox()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AFlagSpawnBox::BeginPlay()
{
}

void AFlagSpawnBox::ServerSpawnFlagActor_Implementation()
{
    if (!HasAuthority()) return;

    if (FlagActorClass) // FlagActor 블루프린트가 지정되어 있다면
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = GetActorLocation(); // 이 액터 자신의 위치
        FRotator SpawnRotation = FRotator::ZeroRotator;

        AFlagActor* FlagActor = GetWorld()->SpawnActor<AFlagActor>(FlagActorClass, SpawnLocation, SpawnRotation, SpawnParams);

        if (FlagActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("FlagActor spawned successfully at location %s"), *SpawnLocation.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("FlagActor spawn failed"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FlagActorClass not set on %s"), *GetName());
    }
}

