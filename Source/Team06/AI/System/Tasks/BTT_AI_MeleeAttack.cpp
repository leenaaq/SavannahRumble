// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_MeleeAttack.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Character/AICharacter.h"

UBTT_AI_MeleeAttack::UBTT_AI_MeleeAttack()
{
	NodeName = TEXT("AI_MeleeAttack");
}

EBTNodeResult::Type UBTT_AI_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		return Result;
	}

	return Result;
}
