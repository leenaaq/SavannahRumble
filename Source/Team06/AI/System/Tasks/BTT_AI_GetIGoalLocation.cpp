// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetIGoalLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_AI_GetIGoalLocation::UBTT_AI_GetIGoalLocation()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTT_AI_GetIGoalLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		/*AGoalItemBase* GoalItem = Cast<AUseItemBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AGoalItemBase::StaticClass()));

		if (GoalItem)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::GoalLocationKey, GoalItem);
			return Result = EBTNodeResult::Succeeded;
		}*/
		
		return Result;
	}

	return Result;
}