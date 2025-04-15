#pragma once

#include "CoreMinimal.h"
#include "Item2/Base/Item2Base.h"
#include "BombItem.generated.h"

UCLASS()
class TEAM06_API ABombItem : public AItem2Base
{
    GENERATED_BODY()

public:
    ABombItem();
    virtual void OnSpawn() override;
    virtual void OnCollision(AActor* OtherActor) override;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastExplode();

protected:
    void Explode();
    FTimerHandle ExplosionTimerHandle;
};
