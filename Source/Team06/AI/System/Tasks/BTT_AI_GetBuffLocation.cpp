// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetBuffLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Item/TriggerItem.h"

UBTT_AI_GetBuffLocation::UBTT_AI_GetBuffLocation()
{
	NodeName = TEXT("GetBuffLocation");
}

EBTNodeResult::Type UBTT_AI_GetBuffLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		TArray<AActor*> BuffActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerItem::StaticClass(), BuffActors);

		if (BuffActors.Num() > 0)
		{
			int32 BuffIndex = FMath::RandRange(0, BuffActors.Num() - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::BuffItemLocationKey, BuffActors[BuffIndex]);

			return EBTNodeResult::Succeeded;
		}
	}

	Result = EBTNodeResult::Failed;
	return Result;
}
