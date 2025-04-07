// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AI_GetBuffLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_AI_GetBuffLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_AI_GetBuffLocation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
