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

	void WaitGameStarter();
	void GameStarter();

	// ---------- Server Function ----------
	UFUNCTION(Server, Reliable)
	void ServerNotifyGoalReached();

	// ---------- AI Character State Check ----------
public:
	bool bIsEquip();

	void SetTargetPlayer(AActor* Player);

	void SetChaseMode(bool value);

	// ---------- Generat Debug Function ----------
public:
	static int32 ShowAIDebug;

	// ---------- Blackboard Key Declaration ----------
public:
	// Object
	static const FName GoalLocationKey;
	static const FName BuffItemLocationKey;
	static const FName UseItemLocationKey;
	static const FName OtherPlayerLocationKey;
	static const FName ClosePlayerKey;

	// Bool
	static const FName ShouldChaseKey;
	static const FName ShouldAttackKey;
	static const FName HaveWeaponKey;

	// Name
	static const FName TargetNameKey;

	// ---------- Behavior Asset ----------
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// ---------- Other Property ----------
protected:
	UPROPERTY(BlueprintReadOnly)
	AActor* TargetPlayer;

	FName NoEquip = "DEFAULT";
};
