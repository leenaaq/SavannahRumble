#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "RespawnableInterface.h"
#include "ItemSpawnPoint.h"
#include "ItemSpawnRow.h"
#include "ItemSpawner.generated.h"

UCLASS()
class TEAM06_API AItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

	void SpawnItems();
	void OnItemDestroyed(AActor* DestroyedActor);
	void RespawnItem(AItemSpawnPoint* SpawnPoint);

	FItemSpawnRow* GetRandomItem() const;

public:
    UPROPERTY(VisibleAnywhere)
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* SpawnArea;

    UPROPERTY(EditDefaultsOnly)
    UDataTable* ItemDataTable;

    UPROPERTY(EditAnywhere)
    float RespawnTime = 5.0f;

private:
    TArray<AItemSpawnPoint*> SpawnPoints;
    TMap<AItemSpawnPoint*, AActor*> SpawnedItems;
};