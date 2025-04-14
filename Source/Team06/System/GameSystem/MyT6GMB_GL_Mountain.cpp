// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/MyT6GMB_GL_Mountain.h"
#include "System/GameSystem/MyT6GSB_GL_Mountain.h"
void AMyT6GMB_GL_Mountain::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AMyT6GSB_GL_Mountain* GS = GetGameState<AMyT6GSB_GL_Mountain>();
    if (GS)
    {
        GS->AddPlayerToArray(NewPlayer);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid GameStateBase called by GMB_Mountain"));
    }
}
