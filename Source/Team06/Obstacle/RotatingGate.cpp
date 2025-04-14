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
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay called"));
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
        UE_LOG(LogTemp, Error, TEXT("🚫 DataTable에서 Row '%s'를 찾을 수 없습니다."), *ObstacleRowName.ToString());
        return;
    }

    if (Row->ObstacleType == "RotatingGate")
    {
        DoorCycle = Row->DoorCycle;
        DoorRotationSpeed = Row->DoorRotationSpeed;
        DoorTargetRotation = Row->DoorRotationAngle;
        DoorStartRotation = GetActorRotation();

        UE_LOG(LogTemp, Warning, TEXT("Row Loaded: Type=%s Cycle=%.2f Speed=%.2f Angle=%s"),
            *Row->ObstacleType.ToString(), Row->DoorCycle, Row->DoorRotationSpeed, *Row->DoorRotationAngle.ToString());

    }
}

void ARotatingGate::StartGateRotation()
{
    Multicast_StartGateRotation();
}

void ARotatingGate::Multicast_StartGateRotation_Implementation()
{
    bIsRotating = true;
    UE_LOG(LogTemp, Warning, TEXT("✅ 회전 시작됨 (Multicast_StartGateRotation)"));
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



