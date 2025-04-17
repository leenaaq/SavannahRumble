#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReviveItem.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TEAM06_API AReviveItem : public AActor
{
    GENERATED_BODY()

public:
    AReviveItem();

protected:
    virtual void BeginPlay() override;
  /*  virtual void Tick(float DeltaTime) override;*/

    UPROPERTY(VisibleAnywhere)
    USceneComponent* RootSceneComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, Category = "ReviveItem")
    FRotator RotationRate = FRotator(0.f, 180.f, 0.f);

    bool bUsed = false;

    UPROPERTY(EditAnywhere, Category = "ReviveItem")
    int32 LifeAmount = 1;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
