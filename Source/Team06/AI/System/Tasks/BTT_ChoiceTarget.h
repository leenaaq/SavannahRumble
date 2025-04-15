// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChoiceTarget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_ChoiceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_ChoiceTarget();

protected:
	void TargetChoseFromEquip(UBehaviorTreeComponent& OwnerComp, int32 Rand, int32 ToPlayer);

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FName Name_Player = "Player";
	FName Name_Item = "Item";
	FName Name_Goal = "Goal";
};
