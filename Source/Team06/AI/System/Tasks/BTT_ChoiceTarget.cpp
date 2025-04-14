// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_ChoiceTarget.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_ChoiceTarget::UBTT_ChoiceTarget()
{
	NodeName = TEXT("ChoiceToChaseTarget");
}

void UBTT_ChoiceTarget::TargetChoseFromEquip(UBehaviorTreeComponent& OwnerComp, int32 Rand, int32 ToPlayer)
{
	if (Rand <= ToPlayer)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsName(AAIC_Enemy::TargetNameKey, Name_Player);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsName(AAIC_Enemy::TargetNameKey, Name_Item);
	}
}

EBTNodeResult::Type UBTT_ChoiceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* AIC = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		int32 RandomNum = FMath::RandRange(1, 100);
		if (RandomNum <= 40)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsName(AAIC_Enemy::TargetNameKey, Name_Goal);
			return Result;
		}
		else
		{
			if (AIC->bIsEquip())
			{
				TargetChoseFromEquip(OwnerComp, RandomNum, 75);
				return Result;
			}
			else
			{
				TargetChoseFromEquip(OwnerComp, RandomNum, 65);
				return Result;
			}
		}
	}

	Result = EBTNodeResult::Failed;
	return Result;
}