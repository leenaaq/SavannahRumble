#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle/ObstacleDataRow.h"
#include "FallingGround.generated.h"

UCLASS()
class TEAM06_API AFallingGround : public AActor
{
    GENERATED_BODY()

public:
    AFallingGround();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ObstacleRowName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* ObstacleDataTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* GroundMesh;

private:
    FVector OriginalLocation;

    float FallDelayTime = 0.0f;
    float FallImpulse = 0.0f;
    float FallDestroyAfter = 0.0f;

    FTimerHandle ShakeTimerHandle;
    FTimerHandle FallTimerHandle;
    FTimerHandle DestroyTimerHandle;

    void ApplyObstacleData(const FObstacleDataRow* Row);
    void StartShake();
    void ApplyShake();
    void StartFalling();
    void ScheduleDestroy();

    UFUNCTION(NetMulticast, Unreliable)
    void Multicast_StartFalling();

};
