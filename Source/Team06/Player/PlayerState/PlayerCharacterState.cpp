#include "Player/PlayerState/PlayerCharacterState.h"
#include "Net/UnrealNetwork.h"
#include "Player/Player/PlayerBase.h"



void APlayerCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerSkinName);
}

void APlayerCharacterState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay: PlayerSkinName = %s"), *PlayerSkinName.ToString());
}

void APlayerCharacterState::OnRep_PlayerSkinName()
{
	
	APlayerBase* PlayerPawn = GetPawn<APlayerBase>();

	if (PlayerPawn)
	{
		PlayerPawn->SetSkinName(PlayerSkinName);
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacterState PlayerSkinName Changed: %s"), *PlayerSkinName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Get Access To Pawn"));
	}
}
