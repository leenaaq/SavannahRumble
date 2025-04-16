// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GetClosePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_GetClosePlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_GetClosePlayer();

protected:
	void SerchTick();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle SerchTickHandle;

	AAIController* CachedAIController = nullptr;
};
