// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_GetPlayerLocation.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "kismet/GameplayStatics.h"
#include "Player/Player/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

UBTT_GetPlayerLocation::UBTT_GetPlayerLocation()
{
	NodeName = TEXT("GetOtherPlayerLocation");
}

EBTNodeResult::Type UBTT_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	bTaskFinished = false;

	CachedOwnerComp = &OwnerComp;
	CachedAIController = OwnerComp.GetAIOwner();
	if (!CachedAIController) return EBTNodeResult::Failed;

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		TArray<AActor*> PlayerActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), PlayerActors);

		if (PlayerActors.Num() > 0)
		{
			int32 PlayerIndex = FMath::RandRange(0, PlayerActors.Num() - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::OtherPlayerLocationKey, PlayerActors[PlayerIndex]);

			FAIRequestID RequestID = CachedAIController->MoveToActor(PlayerActors[PlayerIndex], AcceptRadius, true, true, false, nullptr, true);
			if (RequestID.IsValid())
			{
				CachedAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTT_GetPlayerLocation::OnMoveCompleted);

				float FinalTimeoutDuration = TimeoutDuration + FMath::FRandRange(-ExtraTimeoutDuration, ExtraTimeoutDuration);
				CachedAIController->GetWorldTimerManager().SetTimer(TimeoutHandle, this, &UBTT_GetPlayerLocation::HandleTimeout, FinalTimeoutDuration, false);
				return EBTNodeResult::InProgress;
			}
		}
	}

	Result = EBTNodeResult::Failed;
	return Result;
}

void UBTT_GetPlayerLocation::TaskFinished(EBTNodeResult::Type TaskResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTaskFinished: Function Start!"));
	if (CachedAIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnTaskFinished: CachedAIController is valid"));
		CachedAIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
		CachedAIController->GetWorldTimerManager().ClearTimer(TimeoutHandle);

		FinishLatentTask(*CachedOwnerComp, TaskResult);
	}
}

void UBTT_GetPlayerLocation::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
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

void UBTT_GetPlayerLocation::HandleTimeout()
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
