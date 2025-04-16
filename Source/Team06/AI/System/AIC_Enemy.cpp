// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI/Character/AICharacter.h"
#include "TimerManager.h"
#include "System/GameSystem/T6GameStateBase.h"

// ---------- Blackboard Key Declaration ----------
const FName AAIC_Enemy::GoalLocationKey(TEXT("GoalLocation"));
const FName AAIC_Enemy::BuffItemLocationKey(TEXT("BuffItemLocation"));
const FName AAIC_Enemy::UseItemLocationKey(TEXT("UseItemLocation"));
const FName AAIC_Enemy::OtherPlayerLocationKey(TEXT("OtherPlayerLocation"));
const FName AAIC_Enemy::ClosePlayerKey(TEXT("ClosePlayer"));

const FName AAIC_Enemy::ShouldChaseKey(TEXT("ShouldChase"));
const FName AAIC_Enemy::ShouldAttackKey(TEXT("ShouldAttack"));
const FName AAIC_Enemy::HaveWeaponKey(TEXT("HaveWeapon"));

const FName AAIC_Enemy::TargetNameKey(TEXT("TargetName"));
// ---------- ////////////////////////// ----------

// ---------- Generat Debug Function ----------
int32 AAIC_Enemy::ShowAIDebug(0);

FAutoConsoleVariableRef CVarShowAIDebug(
	TEXT("AI_System_Minjae.ShowAIDebug"),
	AAIC_Enemy::ShowAIDebug,
	TEXT(""),
	ECVF_Cheat
);
// ---------- ///////////////////// ----------

AAIC_Enemy::AAIC_Enemy()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));

	bWantsPlayerState = true;
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
	}
}

void AAIC_Enemy::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent) == true)
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent) == true)
		{
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
		}

		BrainComponent->StopLogic(TEXT("GameStartWaiting"));
	}
}

void AAIC_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndAI();

	Super::EndPlay(EndPlayReason);
}

void AAIC_Enemy::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();
	}
}

void AAIC_Enemy::WaitGameStarter()
{
	FTimerHandle TimeoutHandle;
	GetWorldTimerManager().SetTimer(TimeoutHandle, this, &AAIC_Enemy::GameStarter, 0.5f, false);
}

void AAIC_Enemy::GameStarter()
{
	AT6GameStateBase* GameState = Cast<AT6GameStateBase>(GetWorld()->GetGameState());
	if (GameState)
	{
		EMatchState NowMatchState = GameState->MatchState;
		if (NowMatchState == EMatchState::Playing)
		{
			GetBrainComponent()->StartLogic();
		}
		else
		{
			WaitGameStarter();
		}
	}
}

// ---------- AI Character State Check ----------
bool AAIC_Enemy::bIsEquip()
{
	// Check AI Character has Equip an item
	AAICharacter* AICharacter = Cast<AAICharacter>(GetPawn());
	if (AICharacter)
	{
		const FName EquippedItem = AICharacter->GetCurrentEquippedItemName();
		if (EquippedItem.IsNone() || EquippedItem == FName("DEFAULT"))
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}

void AAIC_Enemy::SetTargetPlayer(AActor* Player)
{
	if (!Player)
	{
		TargetPlayer = nullptr;
	}
	else
	{
		TargetPlayer = Player;
	}
}

void AAIC_Enemy::SetChaseMode(bool value)
{
	GetBlackboardComponent()->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, value);
}
// ---------- //////////////////////// ----------