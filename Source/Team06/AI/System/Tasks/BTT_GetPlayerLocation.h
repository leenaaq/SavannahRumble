// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_GetPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_GetPlayerLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	

};
