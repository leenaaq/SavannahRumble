// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_MoveJump.generated.h"

class AAIC_Enemy;

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_MoveJump : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_MoveJump();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void TaskFinished(EBTNodeResult::Type TaskResult);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void HandleTimeout();

	FTimerHandle TimeoutHandle;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float TimeoutDuration = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI")
	float ExtraTimeoutDuration = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptRadius = 150.f;

	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
	AAIC_Enemy* CachedAIController = nullptr;

	bool bTaskFinished;
};
