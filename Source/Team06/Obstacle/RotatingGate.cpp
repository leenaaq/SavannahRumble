#include "Obstacle/RotatingGate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

ARotatingGate::ARotatingGate()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    ObstacleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObstacleMeshComponent"));
    ObstacleMeshComponent->SetupAttachment(SceneRoot);
    ObstacleMeshComponent->SetRelativeLocation(FVector::ZeroVector);
    ObstacleMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);

    ObstacleRowName = "RotatingGate";

}

void ARotatingGate::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        if (ObstacleDataTable)
        {
            static const FString Context = TEXT("RotatingGate");
            const FObstacleDataRow* Row = ObstacleDataTable->FindRow<FObstacleDataRow>(ObstacleRowName, Context);
            ApplyObstacleData(Row);

            if (Row && Row->ObstacleType == "RotatingGate")
            {
                GetWorldTimerManager().SetTimer(
                    GateCycleTimerHandle,
                    this,
                    &ARotatingGate::StartGateRotation,
                    DoorCycle,
                    false
                );
            }
        }
    }
}

void ARotatingGate::ApplyObstacleData(const FObstacleDataRow* Row)
{
    if (!Row)
    {
        return;
    }

    if (Row->ObstacleType == "RotatingGate")
    {
        DoorCycle = Row->DoorCycle;
        DoorRotationSpeed = Row->DoorRotationSpeed;
        DoorTargetRotation = Row->DoorRotationAngle;
        DoorStartRotation = GetActorRotation();
    }
}

void ARotatingGate::StartGateRotation()
{
    Multicast_StartGateRotation();
}

void ARotatingGate::Multicast_StartGateRotation_Implementation()
{
    bIsRotating = true;
}

void ARotatingGate::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority() && bIsRotating)
    {
        FRotator CurrentRotation = SceneRoot->GetRelativeRotation();
        FRotator TargetRotation = DoorStartRotation + DoorTargetRotation;

        FRotator NewRotation = FMath::RInterpConstantTo(
            CurrentRotation,
            TargetRotation,
            DeltaTime,
            DoorRotationSpeed
        );

        SceneRoot->SetRelativeRotation(NewRotation);
        Multicast_UpdateRotation(NewRotation);

        if (NewRotation.Equals(TargetRotation, 0.1f))
        {
            bIsRotating = false;
        }
    }
}

void ARotatingGate::Multicast_UpdateRotation_Implementation(const FRotator& NewRotation)
{
    if (!HasAuthority())
    {
        SceneRoot->SetRelativeRotation(NewRotation);
    }
}



