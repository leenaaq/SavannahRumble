// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetItemLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_AI_GetItemLocation::UBTT_AI_GetItemLocation()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTT_AI_GetItemLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		/*AUseItemBase* UseItem = Cast<AUseItemBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AUseItemBase::StaticClass()));

		if (UseItem)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::UseItemLocationKey, UseItem);
			return Result = EBTNodeResult::Succeeded;
		}*/

		return Result;
	}

	return Result;
}
