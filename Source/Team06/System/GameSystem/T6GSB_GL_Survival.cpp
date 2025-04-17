// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/T6GSB_GL_Survival.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h" // TActorIterator
#include "Engine/PlayerStartPIE.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "GameFramework/Controller.h"
#include "Player/Controller/PCController_GamePlay.h"
void AT6GSB_GL_Survival::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, AliveSurviverCount);
}

void AT6GSB_GL_Survival::InitiatePlayerLife(int32 Life)
{
	for (TActorIterator<AController> It(GetWorld()); It; ++It)
	{
		AController* Controller = *It;
		if (Controller)
		{
            if (!PlayerLives.Contains(Controller))
            {
                UE_LOG(LogTemp, Warning, TEXT("AController Life Added"));
                PlayerLives.Add(Controller, Life);
            }
            else
            {
               // UE_LOG(LogTemp, Warning, TEXT("Alreay Registered AController"));
            }
		}
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Can't Add AController"));
        }
	}
}

void AT6GSB_GL_Survival::InitSpawnPoint()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		SpawnPoints.Add(*It); // *It으로 액터 포인터 추가
	}
}

void AT6GSB_GL_Survival::RespawnPlayer(AController* Player)
{
    if (!Player) return;

    APawn* StoredPawn = nullptr;
    if (RespawningPawn.Contains(Player))
    {
        StoredPawn = RespawningPawn[Player];
        RespawningPawn.Remove(Player);
    }

    if (APCController_GamePlay* PCCGP = Cast<APCController_GamePlay>(Player))
    {
        PCCGP->Server_EndRespawnUI();
    }

    if (StoredPawn)
    {
        if (IsValid(SpawnPoints[0]))
        {
            if (APlayerController* PC = Cast<APlayerController>(Player))
            {
                PC->ChangeState(NAME_Playing);
                PC->PlayerState->SetIsSpectator(false);
                PC->PlayerState->SetIsOnlyASpectator(false);
                PC->bPlayerIsWaiting = true;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Can Not Reastore PlayerController Setting"));
            }
            // 다시 Possess
            Player->Possess(StoredPawn);

            if (!IsValid(Player->GetPawn()))
            {
                UE_LOG(LogTemp, Error, TEXT("Controller Pawn Possed faild!!!!!!"));
                return;
            }
            FVector NewLocation = SpawnPoints[0]->GetActorLocation();
            NewLocation += FVector(FMath::FRandRange(0, 50.f), FMath::FRandRange(0, 50.f), FMath::FRandRange(0, 10.f));

            // Pawn 위치 이동 후 복원
            StoredPawn->SetActorLocation(NewLocation);
            StoredPawn->SetActorRotation(SpawnPoints[0]->GetActorRotation());

            // Pawn 복구
            StoredPawn->SetActorEnableCollision(true);
            StoredPawn->SetActorHiddenInGame(false);
            StoredPawn->EnableInput(Cast<APlayerController>(Player));


        }
    }
}

void AT6GSB_GL_Survival::OnPlayerLifeIncrease_Implementation(AController* Player)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("InvalidLifeIncreaseCalled in GSB_GL_Survival"));
        return;
    }
    else
    {
        ++PlayerLives[Player];
        UE_LOG(LogTemp, Warning, TEXT("Player %s 's Life Increased!!! "),*Player->PlayerState->GetPlayerName());
    }
}


