#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Obstacle/ObstacleDataRow.h"
#include "RotatingGate.generated.h"

UCLASS()
class TEAM06_API ARotatingGate : public AActor
{
    GENERATED_BODY()

public:
    ARotatingGate();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_UpdateRotation(const FRotator& NewRotation);

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RotatingGate")
    FName ObstacleRowName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RotatingGate")
    UDataTable* ObstacleDataTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ObstacleMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* SceneRoot;

private:
    FTimerHandle GateCycleTimerHandle;

    UPROPERTY()
    float DoorCycle;

    UPROPERTY()
    float DoorRotationSpeed;

    UPROPERTY()
    FRotator DoorTargetRotation;

    UPROPERTY()
    FRotator DoorStartRotation;

    UPROPERTY()
    bool bIsRotating; 

    void ApplyObstacleData(const FObstacleDataRow* Row);
    void StartGateRotation();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartGateRotation();
};
