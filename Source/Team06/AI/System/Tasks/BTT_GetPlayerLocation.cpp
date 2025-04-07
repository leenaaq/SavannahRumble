// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_GetPlayerLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Player/PlayerCharacter.h"

UBTT_GetPlayerLocation::UBTT_GetPlayerLocation()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
