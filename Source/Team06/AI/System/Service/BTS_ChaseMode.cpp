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
	Interval = ServiceInterval + ServiceIntervalExtra;
}

void UBTS_ChaseMode::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (!IsValid(AIController)) return;

	AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
	if (!AICharacter) return;

	ABC = true;
		if (ABC == true)
		{
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			if (!BBComp) return;

			BBComp->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, true);
			ABC = false;
		}
		else
		{
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			if (!BBComp) return;

			BBComp->SetValueAsBool(AAIC_Enemy::ShouldChaseKey, false);

			ABC = true;
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