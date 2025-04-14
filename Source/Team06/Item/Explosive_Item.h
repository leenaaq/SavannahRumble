// Explosive_Item.h
#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Explosive_Item.generated.h"

class USphereComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class TEAM06_API AExplosive_Item : public AEquipableItem
{
    GENERATED_BODY()

public:
    AExplosive_Item();

protected:
    virtual void BeginPlay() override;

    virtual void OnItemPickedUp(AActor* OtherActor);

    UFUNCTION()
    void Explode();

    UFUNCTION()
    void ApplyExplosionEffect();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ExplodeEffect();

    void DestroyItem();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Component")
    TObjectPtr<USphereComponent> ExplosionArea;

    UPROPERTY(EditDefaultsOnly, Category = "Explosion")
    float ExplosionDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Explosion")
    float ExplosionRadius = 400.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Explosion")
    float Damage = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Explosion")
    float KnockbackStrength = 1200.0f;

    UPROPERTY(EditAnywhere, Category = "Explosion|Effect")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

    UPROPERTY(EditAnywhere, Category = "Explosion|Sound")
    TObjectPtr<USoundBase> ExplosionSound;

private:
    FTimerHandle ExplosionTimerHandle;
    FTimerHandle DestroyHandle;
};