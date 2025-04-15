#include "Item2/Bomb/BombItem.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "DrawDebugHelpers.h"
#include "Player/Player/PlayerBase.h"

ABombItem::ABombItem()
{
    bReplicates = true;
    SetReplicateMovement(true);
}

void ABombItem::OnSpawn()
{
    Super::OnSpawn();
    if (HasAuthority() && ExplosionDelay > 0.f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            ExplosionTimerHandle,
            this,
            &ABombItem::Explode,
            ExplosionDelay,
            false
        );
    }
}

void ABombItem::OnCollision(AActor* OtherActor)
{
    if (!HasAuthority() || !OtherActor) return;
    if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
    {
        Player->OnStunned();
        ApplyEffect(Player);
    }
    GetWorld()->GetTimerManager().ClearTimer(ExplosionTimerHandle);
    Explode();
}

void ABombItem::Explode()
{
    if (!HasAuthority()) return;
    MulticastExplode();
    Destroy();
}

void ABombItem::MulticastExplode_Implementation()
{
    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        ExplosionRadius,
        32,
        FColor::Red,
        false,
        3.0f
    );

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        Damage,
        GetActorLocation(),
        ExplosionRadius,
        UDamageType::StaticClass(),
        TArray<AActor*>(),
        this,
        GetInstigatorController(),
        true
    );

    TArray<AActor*> Overlapping;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), Overlapping);
    for (AActor* A : Overlapping)
    {
        APlayerBase* P = Cast<APlayerBase>(A);
        if (!P) continue;
        float Dist = FVector::Dist(GetActorLocation(), P->GetActorLocation());
        if (Dist > ExplosionRadius) continue;
        FVector Dir = (P->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        float Strength = KnockBackMultiplier * 1000.f * (1.f - Dist / ExplosionRadius);
        FVector Vel = Dir * Strength + FVector(0, 0, Strength * 0.5f);
        P->LaunchCharacter(Vel, true, true);
    }
}
