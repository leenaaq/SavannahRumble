#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemType.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class TEAM06_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	AItemSpawnPoint();

protected:
	virtual void BeginPlay() override;

	// 아이템 스폰
	void SpawnItem();

public:
	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<TSubclassOf<AActor>> EquipableItemClass;

	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<TSubclassOf<AActor>> TriggerItemClasses;

	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType SpawnItemType = EItemType::Trigger;

	UPROPERTY(EditAnywhere, Category = "Item")
	FVector SpawnOffset = FVector::ZeroVector;
};
