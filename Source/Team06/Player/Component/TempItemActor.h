#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TempItemActor.generated.h"

class USphereComponent;

UCLASS()
class TEAM06_API ATempItemActor : public AActor
{
    GENERATED_BODY()

public:
    ATempItemActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    USphereComponent* SphereComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

    virtual void BeginPlay() override;

    UFUNCTION()
    void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
