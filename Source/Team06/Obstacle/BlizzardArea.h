#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "ObstacleDataRow.h"
#include "BlizzardArea.generated.h"

UCLASS()
class TEAM06_API ABlizzardArea : public AActor
{
    GENERATED_BODY()

public:
    ABlizzardArea();

protected:
    virtual void BeginPlay() override;

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UAudioComponent* BlizzardAudio;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blizzard Area")
    USoundBase* BlizzardSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ObstacleRowName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* ObstacleDataTable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBoxComponent* DamageArea;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* VisualMesh;

private:
    float BlizzardTickInterval = 1.0f;
    float BlizzardDamage = 1.0f;

    FTimerHandle DamageTimerHandle;
    TSet<AActor*> OverlappingPlayers;

    void ApplyObstacleData(const FObstacleDataRow* Row);
    void DealDamageToPlayers();

    UFUNCTION()
    void OnDamageAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnDamageAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayBlizzardSound();
};
