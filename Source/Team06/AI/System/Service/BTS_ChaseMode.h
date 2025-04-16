// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ChaseMode.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTS_ChaseMode : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_ChaseMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float ServiceInterval;
};
