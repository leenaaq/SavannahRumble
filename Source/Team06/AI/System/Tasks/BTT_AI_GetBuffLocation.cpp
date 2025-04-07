// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetBuffLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_AI_GetBuffLocation::UBTT_AI_GetBuffLocation()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTT_AI_GetBuffLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		/*ABuffItemBase* BuffItem = Cast<AUseItemBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ABuffItemBase::StaticClass()));

		if (BuffItem)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::BuffItemLocationKey, BuffItem);
			return Result = EBTNodeResult::Succeeded;
		}*/

		return Result;
	}

	return Result;
}
