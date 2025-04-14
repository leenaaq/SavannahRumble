// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_GetItemLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/EquipableItem.h"

UBTT_AI_GetItemLocation::UBTT_AI_GetItemLocation()
{
	NodeName = TEXT("GetEquipItemLocation");
}

EBTNodeResult::Type UBTT_AI_GetItemLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		TArray<AActor*> ItemActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEquipableItem::StaticClass(), ItemActors);

		if (ItemActors.Num() > 0)
		{
			int32 ItemIndex = FMath::RandRange(0, ItemActors.Num() - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::UseItemLocationKey, ItemActors[ItemIndex]);

			return Result;
		}
	}

	Result = EBTNodeResult::Failed;
	return Result;
}
