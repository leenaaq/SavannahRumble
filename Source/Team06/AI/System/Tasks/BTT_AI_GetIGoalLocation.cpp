// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetIGoalLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Component/FlagActor.h"
#include "System/GameSystem/FinishGoalActor.h"

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
		AActor* GoalActor = UGameplayStatics::GetActorOfClass(GetWorld(), AFlagActor::StaticClass());
		if (GoalActor)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::GoalLocationKey, GoalActor);

			return Result;
		}

		AActor* MountainGoal = UGameplayStatics::GetActorOfClass(GetWorld(), AFinishGoalActor::StaticClass());
		if (MountainGoal)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::GoalLocationKey, MountainGoal);

			return Result;
		}
	}

	Result = EBTNodeResult::Failed;
	return Result;
}