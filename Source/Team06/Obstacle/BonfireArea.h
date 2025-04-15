#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "ObstacleDataRow.h"
#include "BonfireArea.generated.h"

UCLASS()
class TEAM06_API ABonfireArea : public AActor
{
    GENERATED_BODY()

public:
    ABonfireArea();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* ProtectionZone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ObstacleRowName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* ObstacleDataTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealInterval = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealAmount = 0.0f;

    bool IsPlayerInside(AActor* Player) const;

private:
    FTimerHandle HealTimerHandle;

    void ApplyObstacleData(const FObstacleDataRow* Row);

    void HealPlayersInZone();
};
