// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AI_GetIGoalLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_AI_GetIGoalLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_AI_GetIGoalLocation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
