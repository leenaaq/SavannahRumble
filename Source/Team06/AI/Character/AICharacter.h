// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Player/PlayerBase.h"
#include "AICharacter.generated.h"

class USphereComponent;
class AFlagActor;

/**
 * 
 */
UCLASS()
class TEAM06_API AAICharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AAICharacter();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	// ---------- Add Components ----------
protected:
	USphereComponent* DetectingFlagCollision;

	// ---------- Proerty Declaration ----------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectorRadius;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName = "spine_03";

	// ---------- Other Property ----------
private:
	AFlagActor* MyFlag = nullptr;

	bool bHasAnyEquip = false;
};
