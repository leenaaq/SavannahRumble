#include "Obstacle/FallingGround.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"

AFallingGround::AFallingGround()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
    SetRootComponent(GroundMesh);

    GroundMesh->SetSimulatePhysics(false);
    GroundMesh->SetEnableGravity(false);
    GroundMesh->SetMobility(EComponentMobility::Movable);
}

void AFallingGround::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (!ObstacleDataTable)
        {
            return;
        }

        const FObstacleDataRow* Row = ObstacleDataTable->FindRow<FObstacleDataRow>(ObstacleRowName, TEXT("FallingGround"));
        if (!Row || Row->ObstacleType != "FallingGround")
        {
            return;
        }

        ApplyObstacleData(Row);

        OriginalLocation = GetActorLocation();

        float ShakeStart = FMath::Max(0.f, FallDelayTime - 1.0f);
        GetWorldTimerManager().SetTimer(ShakeTimerHandle, this, &AFallingGround::StartShake, ShakeStart, false);
        GetWorldTimerManager().SetTimer(FallTimerHandle, this, &AFallingGround::StartFalling, FallDelayTime, false);
    }
}

void AFallingGround::ApplyObstacleData(const FObstacleDataRow* Row)
{
    if (!Row) return;

    FallDelayTime = Row->FallDelayTime;
    FallImpulse = Row->FallImpulse;
    FallDestroyAfter = Row->FallDestroyAfter;
}


void AFallingGround::StartShake()
{
    OriginalLocation = GetActorLocation();

    const float ShakeDuration = 1.0f;
    const float ShakeInterval = 0.05f;
    const int32 RepeatCount = FMath::FloorToInt(ShakeDuration / ShakeInterval);

    for (int32 i = 0; i < RepeatCount; ++i)
    {
        float Delay = i * ShakeInterval;
        FTimerHandle TempHandle;
        GetWorldTimerManager().SetTimer(TempHandle, this, &AFallingGround::ApplyShake, Delay, false);
    }
}

void AFallingGround::ApplyShake()
{
    FVector Offset = FVector(
        FMath::RandRange(-5.f, 5.f),
        FMath::RandRange(-5.f, 5.f),
        0.f
    );
    SetActorLocation(OriginalLocation + Offset);
}

void AFallingGround::StartFalling()
{
    SetActorLocation(OriginalLocation);
    Multicast_StartFalling();

    if (FallDestroyAfter > 0.0f)
    {
        GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AFallingGround::ScheduleDestroy, FallDestroyAfter, false);
    }
}


void AFallingGround::Multicast_StartFalling_Implementation()
{

    GroundMesh->SetMobility(EComponentMobility::Movable);

    GroundMesh->BodyInstance.bLockXRotation = true;
    GroundMesh->BodyInstance.bLockYRotation = true;
    GroundMesh->BodyInstance.bLockZRotation = true;

    GroundMesh->SetSimulatePhysics(true);
    GroundMesh->SetEnableGravity(true);

    FTimerHandle TempHandle;
    GetWorldTimerManager().SetTimer(TempHandle, [this]()
        {
            if (FallImpulse > 0.f)
            {
                GroundMesh->AddImpulse(FVector(0, 0, -FallImpulse), NAME_None, true);
            }
        }, 0.01f, false);
}


void AFallingGround::ScheduleDestroy()
{
    if (HasAuthority())
    {
        Destroy();
    }
}
