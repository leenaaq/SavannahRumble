// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GSB_GL_FlagMap.h"
#include "Engine/PlayerStartPIE.h"
#include "EngineUtils.h"
#include "System/GameSystem/FlagSpawnBox.h"


void AT6GSB_GL_FlagMap::BeginPlay()
{
    Super::BeginPlay(); // 부모 클래스 BeginPlay 호출 잊지 마세요
}

void AT6GSB_GL_FlagMap::InitSpawnPoint()
{
    // PlayerStart 모으기
    for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
    {
        SpawnPoints.Add(*It); // *It으로 액터 포인터 추가
    }

    // FlagSpawnBox 하나만 저장
    for (TActorIterator<AFlagSpawnBox> It(GetWorld()); It; ++It)
    {
        FlagSpawnPoint = *It;
        break; // 첫 번째 찾은 FlagSpawnBox만 사용
    }
}

void AT6GSB_GL_FlagMap::SpawnFlagActor()
{
    if (FlagSpawnPoint)
    {
        FlagSpawnPoint->ServerSpawnFlagActor();
    }
    else
    {
        UE_LOG(LogTemp,Error,TEXT("No FlagSpawnPoint!!!"));
    }
}

AController* AT6GSB_GL_FlagMap::GetFlagOwningController()
{
    return FlagOwningController;
}

void AT6GSB_GL_FlagMap::ServerChangeFlagOwner_Implementation(AController* NewFlagOwner)
{
    FlagOwningController = NewFlagOwner;
}
