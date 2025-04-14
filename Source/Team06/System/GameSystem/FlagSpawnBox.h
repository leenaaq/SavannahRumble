// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagSpawnBox.generated.h"


class AFlagActor;
UCLASS()
class TEAM06_API AFlagSpawnBox : public AActor
{
	GENERATED_BODY()
public:
	AFlagSpawnBox();
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
public:
    UFUNCTION(Server, Reliable)
    void ServerSpawnFlagActor();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flag")
    TSubclassOf<AFlagActor> FlagActorClass;

    //UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Flag")
    //AFlagActor* FlagActor = nullptr;
};
