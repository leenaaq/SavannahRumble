// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_GetClosePlayer.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UBTT_GetClosePlayer::UBTT_GetClosePlayer()
{
	NodeName = TEXT("GetMostClosePlayer");
}

void UBTT_GetClosePlayer::SerchTick()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Players);

	if (Players.Num() > 0)
	{
		float NearsetActorDistance = 100.f;
		AActor* NearsetPlayer = UGameplayStatics::FindNearestActor(CachedAIController->GetPawn()->GetActorLocation(), Players, NearsetActorDistance);
	}
}

EBTNodeResult::Type UBTT_GetClosePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	CachedAIController = OwnerComp.GetAIOwner();

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(CachedAIController);
	if (IsValid(AIController))
	{
		OwnerComp.GetAIOwner()->GetWorldTimerManager().SetTimer(SerchTickHandle, this, &UBTT_GetClosePlayer::SerchTick, 0.7f, true);
	}
	else
	{
		Result = EBTNodeResult::Failed;
		return Result;
	}

	Result = EBTNodeResult::Succeeded;
	return Result;
}

