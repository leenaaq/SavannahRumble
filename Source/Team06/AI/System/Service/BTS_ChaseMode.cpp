// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Service/BTS_ChaseMode.h"
#include "AI/Character/AICharacter.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Player/PlayerCharacter.h"
#include "Engine/OverlapResult.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UBTS_ChaseMode::UBTS_ChaseMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("ChaseModeChanger");
	Interval = ServiceInterval;
}

void UBTS_ChaseMode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController)) return;

	AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
	if (!AICharacter) return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;

	if (AIController->bIsEquip())
	{
		BBComp->SetValueAsBool(AAIC_Enemy::HaveWeaponKey, true);

		bool bShouldAttack = false;
		int32 RandomNum = FMath::RandRange(1, 100);
		if (RandomNum <= 80)
		{
			bShouldAttack = true;
		}
		else
		{
			bShouldAttack = false;
		}

		BBComp->SetValueAsBool(AAIC_Enemy::ShouldAttackKey, bShouldAttack);
	}
	else
	{
		BBComp->SetValueAsBool(AAIC_Enemy::HaveWeaponKey, false);

		bool bShouldAttack = false;
		int32 RandomNum = FMath::RandRange(1, 100);
		if (RandomNum <= 80)
		{
			bShouldAttack = true;
		}
		else
		{
			bShouldAttack = false;
		}

		BBComp->SetValueAsBool(AAIC_Enemy::ShouldAttackKey, bShouldAttack);
	}

	bool bShouldChase = false;
	int32 RandomNum = FMath::RandRange(1, 100);
	if (RandomNum <= 50)
	{
		bShouldChase = true;
	}
	else
	{
		int32 ChaseChance = AIController->bIsEquip() ? 90 : 60;
		bShouldChase = (RandomNum <= ChaseChance);
	}

	if (AICharacter->GetbHasFlag())
	{
		bShouldChase = false;
	}

	if (bShouldChase)
	{
		BBComp->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, true);
	}
	else
	{
		BBComp->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, false);
	}



	/*AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController)) return;

	AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
	if (!AICharacter) return;

	bool bShouldChase = false;
	int32 RandomNum = FMath::RandRange(1, 100);
	if (RandomNum <= 50)
	{
		bShouldChase = true;
	}
	else
	{
		int32 ChaseChance = AIController->bIsEquip() ? 90 : 60;
		bShouldChase = (RandomNum <= ChaseChance);
	}

	if (AICharacter->GetbHasFlag())
	{
		bShouldChase = false;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, bShouldChase);*/
}