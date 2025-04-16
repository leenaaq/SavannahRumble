// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_MoveJump.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UBTT_MoveJump::UBTT_MoveJump()
{
	NodeName = TEXT("MoveToTargetAndJump");
}

EBTNodeResult::Type UBTT_MoveJump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	bTaskFinished = false;

	CachedOwnerComp = &OwnerComp;
	CachedAIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (!CachedAIController) return EBTNodeResult::Succeeded;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Succeeded;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Succeeded;

	FAIRequestID RequestID = CachedAIController->MoveToActor(TargetActor, AcceptRadius, true, true, false, nullptr, true);
	if (RequestID.IsValid())
	{
		CachedAIController->SetTargetPlayer(TargetActor); // 선택적으로 유지

		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_MoveJump::OnMoveCompleted);

		float FinalTimeoutDuration = TimeoutDuration + FMath::FRandRange(-ExtraTimeoutDuration, ExtraTimeoutDuration);
		CachedAIController->GetWorldTimerManager().SetTimer(TimeoutHandle, this, &UBTT_MoveJump::HandleTimeout, FinalTimeoutDuration, false);
		return EBTNodeResult::InProgress;
	}
	else
	{
		CachedAIController->SetTargetPlayer(nullptr);
		return EBTNodeResult::Succeeded;
	}

	/*FAIRequestID RequestID = CachedAIController->MoveToActor(PlayerActors[PlayerIndex], AcceptRadius, true, true, false, nullptr, true);
	if (RequestID.IsValid())
	{
		AIController->SetTargetPlayer(PlayerActors[PlayerIndex]);

		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_GetPlayerLocation::OnMoveCompleted);

		float FinalTimeoutDuration = TimeoutDuration + FMath::FRandRange(-ExtraTimeoutDuration, ExtraTimeoutDuration);
		CachedAIController->GetWorldTimerManager().SetTimer(TimeoutHandle, this, &UBTT_GetPlayerLocation::HandleTimeout, FinalTimeoutDuration, false);
		return EBTNodeResult::InProgress;
	}
	else
	{
		AIController->SetTargetPlayer(nullptr);
		Result = EBTNodeResult::Failed;
		return Result;
	}

	return EBTNodeResult::Type();*/
}

void UBTT_MoveJump::TaskFinished(EBTNodeResult::Type TaskResult)
{
	if (CachedAIController)
	{
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		CachedAIController->GetWorldTimerManager().ClearTimer(TimeoutHandle);

		FinishLatentTask(*CachedOwnerComp, TaskResult);
	}
}

void UBTT_MoveJump::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (CachedOwnerComp && CachedAIController)
	{
		CachedAIController->GetWorldTimerManager().ClearTimer(TimeoutHandle);
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

		if (bTaskFinished == false)
		{
			bTaskFinished = true;
			TaskFinished(EBTNodeResult::Succeeded);
		}
	}
}

void UBTT_MoveJump::HandleTimeout()
{
	if (CachedOwnerComp && CachedAIController)
	{
		CachedAIController->GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::ForcedScript);
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);

		if (bTaskFinished == false)
		{
			bTaskFinished = true;
			TaskFinished(EBTNodeResult::Succeeded);
		}
	}
}