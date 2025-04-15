#pragma once

#include "CoreMinimal.h"
#include "Item2/Base/Item2Base.h"
#include "ThrowableSpawnerItem.generated.h"

UCLASS()
class TEAM06_API AThrowableSpawnerItem : public AItem2Base
{
    GENERATED_BODY()

public:
    AThrowableSpawnerItem();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_SpawnActor(const FVector& SpawnLocation, const FRotator& SpawnRotation);

    void SpawnAssignedActor();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
    TSubclassOf<AActor> ActorToSpawn;

    FVector CachedHitLocation;
    FRotator CachedHitRotation;
};