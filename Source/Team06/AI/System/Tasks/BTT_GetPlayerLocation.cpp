// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_GetPlayerLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"

UBTT_GetPlayerLocation::UBTT_GetPlayerLocation()
{
	NodeName = TEXT("GetOtherPlayerLocation");
}

EBTNodeResult::Type UBTT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		TArray<AActor*> PlayerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PlayerActors);

		if (PlayerActors.Num() > 0)
		{
			int32 PlayerIndex = FMath::RandRange(0, PlayerActors.Num() - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::OtherPlayerLocationKey, PlayerActors[PlayerIndex]);

			AIController->SetTargetPlayer(PlayerActors[PlayerIndex]);
		}
	}

	Result = EBTNodeResult::Succeeded;
	return Result;
}
