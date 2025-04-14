#include "Item/Explosive_Item.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"

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

    ApplyExplosionEffect();
    Multicast_ExplodeEffect();

    GetWorld()->GetTimerManager().SetTimer(DestroyHandle, this, &AExplosive_Item::DestroyItem, 0.2f, false);
}

void AExplosive_Item::ApplyExplosionEffect()
{
    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->OverlapMultiByChannel(
        OverlapResults,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn,
        FCollisionShape::MakeSphere(ExplosionRadius),
        QueryParams
    );

    UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 감지된 플레이어 수: %d"), OverlapResults.Num());

    for (const FOverlapResult& Result : OverlapResults)
    {
        APlayerBase* Player = Cast<APlayerBase>(Result.GetActor());
        if (Player && Player->GetHealth() > 0)
        {
            FVector KnockDir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            FVector LaunchVelocity = KnockDir * KnockbackStrength;

            Player->LaunchCharacter(LaunchVelocity, true, true);
            Player->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
            Player->OnStunned();
            UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] %s 데미지: %.1f | 넉백: %.0f"),
                *Player->GetName(), Damage, KnockbackStrength);
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 16, FColor::Red, false, 2.0f);
}

void AExplosive_Item::Multicast_ExplodeEffect_Implementation()
{
    if (ExplosionEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionEffect,
            GetActorLocation(),
            FRotator::ZeroRotator,
            FVector(1.0f),
            true,   // bAutoDestroy = true
            true    // bAutoActivate = true
        );
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