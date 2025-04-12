// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointBox.generated.h"

UCLASS()
class TEAM06_API ACheckpointBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 CheckPointIndex;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector CheckPointLocation;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(Server, Reliable)
    void ServerHandleCheckpoint(APlayerController* PC, int32 ThisCheckpointIndex);
};
