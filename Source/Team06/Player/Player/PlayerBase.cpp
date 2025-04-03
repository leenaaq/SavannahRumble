#include "PlayerBase.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerBase::UpdateStatsFromDataTable()
{
	if (StatsRowHandle.DataTable)
	{
		static const FString ContextString(TEXT("Player stats retrieval"));
		FPlayerStats* Stats = StatsRowHandle.DataTable->FindRow<FPlayerStats>(StatsRowHandle.RowName, ContextString);
		if (Stats)
		{
			PlayerStats = *Stats;
		}
	}
}