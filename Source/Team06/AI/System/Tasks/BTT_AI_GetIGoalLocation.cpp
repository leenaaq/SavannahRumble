// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetIGoalLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_AI_GetIGoalLocation::UBTT_AI_GetIGoalLocation()
{
	NodeName = TEXT("GetGoalLocation");
}

EBTNodeResult::Type UBTT_AI_GetIGoalLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		/*AActor* GoalActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGoalActor::StaticClass());

		if (GoalActor)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::UseItemLocationKey, GoalActor);

			return Result;
		}*/
	}

	Result = EBTNodeResult::Failed;
	return Result;
}