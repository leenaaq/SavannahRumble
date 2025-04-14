#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Obstacle/ObstacleDataRow.h"
#include "PushingObstacle.generated.h"

UCLASS()
class TEAM06_API APushingObstacle : public AActor
{
    GENERATED_BODY()

public:
    APushingObstacle();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ObstacleRowName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* ObstacleDataTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ObstacleMeshComponent;

private:
    FVector StartLocation;
    FVector EndLocation;

    bool bMovingForward = true;
    bool bIsPaused = false;

    float ElapsedMoveTime = 0.0f;

    FTimerHandle DestroyTimerHandle;
    FTimerHandle PauseTimerHandle;

    float PushingSpeed = 0.0f;
    float MoveCycle = 0.0f;
    float MoveDistance = 0.0f;
    float PauseAfterRoundTrip = 0.0f;
    float DestroyAfterSeconds = 0.0f;

    void ApplyObstacleData(const FObstacleDataRow* Row);
    void ToggleDirection();
    void ResumeMovement();
    void ScheduleDestroy();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_UpdatePosition(const FVector& NewLocation);
};
