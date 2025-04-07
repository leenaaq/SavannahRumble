// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AI_Jumping.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_AI_Jumping : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_AI_Jumping();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};