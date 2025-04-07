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
		UE_LOG(LogTemp, Warning, TEXT("아이템 타입이 지정되지 않았습니다."));
		break;
	}

	if (ItemToSpawn)
	{
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(ItemToSpawn, GetActorLocation() + SpawnOffset, GetActorRotation(), SpawnParams);

		UE_LOG(LogTemp, Log, TEXT("아이템이 성공적으로 스폰되었습니다: %s"), *ItemToSpawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("스폰할 아이템이 없습니다."));
	}
}

