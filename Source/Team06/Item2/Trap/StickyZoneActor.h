#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "StickyZoneActor.generated.h"

class UDecalComponent;
class UStaticMeshComponent;
class USphereComponent;
class APlayerBase;

USTRUCT()
struct FMovementBackup
{
    GENERATED_BODY()

    float OriginalSpeed = 0.f;
    float OriginalJump = 0.f;
};

UCLASS()
class TEAM06_API AStickyZoneActor : public AActor
{
    GENERATED_BODY()

public:
    AStickyZoneActor();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ApplySlowEffect(APlayerBase* Player);
    void RemoveSlowEffect(APlayerBase* Player);

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* ZoneTrigger;

    UPROPERTY(VisibleAnywhere)
    UDecalComponent* Decal;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* VisualMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    FDataTableRowHandle DataRowHandle;

    UPROPERTY()
    TSet<APlayerBase*> AffectedPlayers;

    UPROPERTY()
    TMap<APlayerBase*, FMovementBackup> MovementBackupMap;

    float SpeedMultiplier = 1.f;
    float JumpMultiplier = 1.f;
};
