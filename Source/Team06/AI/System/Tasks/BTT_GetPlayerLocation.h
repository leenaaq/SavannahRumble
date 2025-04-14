// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_GetPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API UBTT_GetPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_GetPlayerLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	void TaskFinished(EBTNodeResult::Type TaskResult);

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	void HandleTimeout();

	FTimerHandle TimeoutHandle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float TimeoutDuration = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI")
	float ExtraTimeoutDuration = 2.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptRadius = 150.f;

	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
	AAIController* CachedAIController = nullptr;

	bool bTaskFinished;
};
