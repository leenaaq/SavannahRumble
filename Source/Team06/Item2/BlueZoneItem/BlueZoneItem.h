#pragma once

#include "CoreMinimal.h"
#include "Item2/Base/Item2Base.h"
#include "Item2/Base/RangeItemDataRow.h"
#include "BlueZoneItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class TEAM06_API ABlueZoneItem : public AItem2Base
{
    GENERATED_BODY()

public:
    ABlueZoneItem();

    virtual void OnSpawn() override;
    virtual void Tick(float DeltaSeconds) override;

protected:
    void ActivateZone();
    void ApplyZoneDamage();
    void UpdateVisualSphere();

    UFUNCTION()
    void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnRep_CurrentRadius();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

    UPROPERTY(VisibleAnywhere, Category = Zone)
    USphereComponent* ZoneTrigger;

    UPROPERTY(VisibleAnywhere, Category = Visual)
    UStaticMeshComponent* VisualSphere;

    UPROPERTY(Replicated)
    float DamagePerSecond = 0.f;

    UPROPERTY(Replicated)
    float MaxRadius = 0.f;

    UPROPERTY(Replicated)
    float ExpansionTime = 0.f;

    UPROPERTY(ReplicatedUsing = OnRep_CurrentRadius)
    float CurrentRadius = 0.f;

    bool  bZoneActive = false;
    float ElapsedTime = 0.f;

    FTimerHandle ActivationTimerHandle;
    FTimerHandle DamageTimerHandle;

    TSet<class APlayerBase*> InsidePlayers;

    static constexpr float ActivationDelay = 3.f;
    static constexpr float DamageInterval = 1.f;
    float InitialRadius = 0.f;

};
