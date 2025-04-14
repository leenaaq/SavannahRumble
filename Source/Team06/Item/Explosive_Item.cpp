#include "Item/Explosive_Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DamageEvents.h"


AExplosive_Item::AExplosive_Item()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ItemID = "BP_Explosive_Item";

    ExplosionArea = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionArea"));
    RootComponent = ExplosionArea;
    ExplosionArea->SetSphereRadius(ExplosionRadius);
    ExplosionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionArea->SetCollisionResponseToAllChannels(ECR_Ignore);
    ExplosionArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AExplosive_Item::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive_Item::Explode, ExplosionDelay, false);
        UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] %.1f초 후 폭발 예정"), ExplosionDelay);
    }
}

void AExplosive_Item::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
    bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority() || bIsActivated) return;

    bIsActivated = true;

    ExplosionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 착지 감지, %.1f초 후 폭발"), ExplosionDelay);

    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive_Item::Explode, ExplosionDelay, false);
}

void AExplosive_Item::OnItemPickedUp(AActor* OtherActor)
{
    if (HasAuthority())
    {
        if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
        {
            Player->ServerSetEquippedItemName(ItemID);
        }
    }
}

void AExplosive_Item::Explode()
{
    if (!HasAuthority()) return;

    UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 폭발 실행 (서버)"));

    ExplosionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ExplosionArea->UpdateOverlaps();

    ApplyExplosionEffect();
    Multicast_ExplodeEffect();

    // ApplyExplosionEffect를 지연 호출
    GetWorld()->GetTimerManager().SetTimer(EffectDelayHandle, this, &AExplosive_Item::ApplyExplosionEffect, 0.05f, false);

    // Destroy는 효과 적용 뒤에 해줘야 함 (또는 ApplyExplosionEffect 안으로 이동)
    GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &AExplosive_Item::DestroyItem, 0.1f, false);

}

void AExplosive_Item::ApplyExplosionEffect()
{
    TArray<AActor*> OverlappingActors;
    ExplosionArea->GetOverlappingActors(OverlappingActors, APlayerBase::StaticClass());

    UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 감지된 플레이어 수: %d"), OverlappingActors.Num());

    for (AActor* Actor : OverlappingActors)
    {
        APlayerBase* Player = Cast<APlayerBase>(Actor);
        if (Player && Player->GetHealth() > 0)
        {
            FVector KnockDir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            FVector LaunchVelocity = KnockDir * KnockbackStrength;

            Player->LaunchCharacter(LaunchVelocity, true, true);
            Player->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);

            UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] %s 데미지: %.1f | 넉백: %.0f"),
                *Player->GetName(), Damage, KnockbackStrength);
        }
    }
}

void AExplosive_Item::Multicast_ExplodeEffect_Implementation()
{
    if (ExplosionEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
    }

    if (ExplosionSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 16, FColor::Red, false, 2.0f);
}

void AExplosive_Item::DestroyItem()
{
    Destroy();
}