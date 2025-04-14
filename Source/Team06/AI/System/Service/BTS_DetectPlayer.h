// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_DetectPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTS_DetectPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_DetectPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
