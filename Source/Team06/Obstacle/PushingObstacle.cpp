#include "Obstacle/PushingObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

APushingObstacle::APushingObstacle()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    ObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMeshComponent"));
    ObstacleMeshComponent->SetupAttachment(SceneRoot);
}

void APushingObstacle::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (ObstacleDataTable)
        {
            static const FString Context = TEXT("PushingObstacle");
            const FObstacleDataRow* Row = ObstacleDataTable->FindRow<FObstacleDataRow>(ObstacleRowName, Context);
            ApplyObstacleData(Row);

            StartLocation = GetActorLocation();
            EndLocation = StartLocation + GetActorForwardVector() * MoveDistance;

            if (DestroyAfterSeconds > 0.0f)
            {
                GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &APushingObstacle::ScheduleDestroy, DestroyAfterSeconds, false);
            }
        }
    }
}

void APushingObstacle::ApplyObstacleData(const FObstacleDataRow* Row)
{
    if (!Row || Row->ObstacleType != "PushingObstacle") return;

    if (Row->ObstacleType == "PushingObstacle")
    {
        MoveDistance = Row->PushingMoveDistance;
        PushingSpeed = Row->PushingSpeed;
        PauseAfterRoundTrip = Row->PushingPauseAfterRoundTrip;
        DestroyAfterSeconds = Row->PushingDestroyAfterSeconds;
    }
}

void APushingObstacle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority() || bIsPaused || PushingSpeed <= 0.f)
        return;

    ElapsedMoveTime += DeltaTime;

    float MoveDuration = MoveDistance / PushingSpeed;
    float Alpha = FMath::Clamp(ElapsedMoveTime / MoveDuration, 0.f, 1.f);
    FVector TargetLocation = bMovingForward
        ? FMath::Lerp(StartLocation, EndLocation, Alpha)
        : FMath::Lerp(EndLocation, StartLocation, Alpha);

    SetActorLocation(TargetLocation);
    Multicast_UpdatePosition(TargetLocation);

    if (Alpha >= 1.0f)
    {
        ToggleDirection();
    }
}

void APushingObstacle::ToggleDirection()
{
    bMovingForward = !bMovingForward;
    ElapsedMoveTime = 0.0f;
    bIsPaused = true;

    GetWorldTimerManager().SetTimer(PauseTimerHandle, this, &APushingObstacle::ResumeMovement, PauseAfterRoundTrip, false);
}

void APushingObstacle::ResumeMovement()
{
    bIsPaused = false;
}

void APushingObstacle::ScheduleDestroy()
{
    Destroy();
}

void APushingObstacle::Multicast_UpdatePosition_Implementation(const FVector& NewLocation)
{
    if (!HasAuthority())
    {
        SetActorLocation(NewLocation);
    }
}
