// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Enemy.generated.h"

class UBlackboardData;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class TEAM06_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIC_Enemy();

protected:
	virtual void BeginPlay() override;

	void BeginAI(APawn* InPawn);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void EndAI();

	// ---------- Blackboard Key Declaration ----------
public:
	static const FName GoalLocationKey;
	static const FName BuffItemLocationKey;
	static const FName UseItemLocationKey;
	static const FName OtherPlayerLocationKey;

	// ---------- Behavior Asset ----------
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
