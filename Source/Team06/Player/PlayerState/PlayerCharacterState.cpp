#include "Player/PlayerState/PlayerCharacterState.h"
#include "Net/UnrealNetwork.h"
#include "Player/Player/PlayerBase.h"



void APlayerCharacterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerSkinName);
}

void APlayerCharacterState::OnRep_PlayerSkinName()
{
	
	APlayerBase* PlayerPawn = GetPawn<APlayerBase>();

	if (PlayerPawn)
	{
		PlayerPawn->SetSkinName(PlayerSkinName);
	}
}
