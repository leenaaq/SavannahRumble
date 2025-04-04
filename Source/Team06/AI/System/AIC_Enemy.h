// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIC_Enemy();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void OnPatrolTimerElapsed();

public:
	FTimerHandle PatrolTimerHandle = FTimerHandle();

	static const float PatrolRepeatInterval;

	static const float PatrolRadius;
};
