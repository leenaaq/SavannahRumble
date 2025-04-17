#include "Item2/ThrowableSpawnerItem/ThrowableSpawnerItem.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AThrowableSpawnerItem::AThrowableSpawnerItem()
{
    PrimaryActorTick.bCanEverTick = false;

    SetReplicates(true);
    SetReplicateMovement(true);
    if (MeshComp)
    {
        MeshComp->SetSimulatePhysics(false); // 물리 시뮬레이션 끄기
    }
}

void AThrowableSpawnerItem::BeginPlay()
{
    Super::BeginPlay();

    if (MeshComp)
    {
        MeshComp->SetSimulatePhysics(true);
        MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        MeshComp->SetNotifyRigidBodyCollision(true);
        MeshComp->OnComponentHit.AddDynamic(this, &AThrowableSpawnerItem::OnHit);
    }
}

void AThrowableSpawnerItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority())
        return;

    CachedHitLocation = Hit.ImpactPoint;
    CachedHitRotation = GetActorRotation();

    SpawnAssignedActor();

    Destroy();
}

void AThrowableSpawnerItem::SpawnAssignedActor()
{
    if (!ActorToSpawn)
    {
        return;
    }

    GetWorld()->SpawnActor<AActor>(ActorToSpawn, CachedHitLocation, CachedHitRotation);

    Multicast_SpawnActor(CachedHitLocation, CachedHitRotation);
}

void AThrowableSpawnerItem::Multicast_SpawnActor_Implementation(const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (HasAuthority()) return;

    if (!ActorToSpawn)
    {
        return;
    }

    GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation);
}