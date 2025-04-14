#pragma once

#include "CoreMinimal.h"
#include "Item/EquipableItem.h"
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

    virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
        bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
    virtual void OnItemPickedUp(AActor* OtherActor) override;

private:
    void Explode();                          // 타이머에 의해 서버에서 호출
    void ApplyExplosionEffect();             // 폭발 영향 처리
    void DestroyItem();                      // 폭탄 제거

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ExplodeEffect();          // 클라에 이펙트 실행

protected:
    UPROPERTY(EditDefaultsOnly)
    USphereComponent* ExplosionArea;

    UPROPERTY(EditDefaultsOnly)
    UNiagaraSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* ExplosionSound;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionRadius = 300.f;

    UPROPERTY(EditDefaultsOnly)
    float ExplosionDelay = 1.5f;

    UPROPERTY(EditDefaultsOnly)
    float Damage = 40.f;

    UPROPERTY(EditDefaultsOnly)
    float KnockbackStrength = 1000.f;


private:
    // 타이머 핸들
    FTimerHandle ExplosionTimerHandle;
    FTimerHandle EffectDelayHandle;
    FTimerHandle DestroyHandle;

    // 상태
    bool bIsActivated = false;
};
