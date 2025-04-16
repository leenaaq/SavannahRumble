#include "ItemSpawnPoint.h"
#include "Engine/World.h"
#include "ItemType.h"

AItemSpawnPoint::AItemSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnItem();
}

void AItemSpawnPoint::SpawnItem()
{
	TSubclassOf<AActor> ItemToSpawn = nullptr;

	switch (SpawnItemType)
	{
	case EItemType::Equipable:
		if (EquipableItemClass.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, EquipableItemClass.Num() - 1);
			ItemToSpawn = EquipableItemClass[Index];
		}
		break;

	case EItemType::Trigger:
		if (TriggerItemClasses.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, TriggerItemClasses.Num() - 1);
			ItemToSpawn = TriggerItemClasses[Index];
		}
		break;
		
	default:
		
		break;
	}

	if (ItemToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(ItemToSpawn, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParams);

	}
	else
	{
		
	}
}

