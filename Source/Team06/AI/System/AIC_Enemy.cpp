// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/AIC_Enemy.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

const float AAIC_Enemy::PatrolRepeatInterval(3.f);
const float AAIC_Enemy::PatrolRadius(500.f);

AAIC_Enemy::AAIC_Enemy()
{
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ThisClass::OnPatrolTimerElapsed, PatrolRepeatInterval, true);
}

void AAIC_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(PatrolTimerHandle);

	Super::EndPlay(EndPlayReason);
}

void AAIC_Enemy::OnPatrolTimerElapsed()
{
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (IsValid(NavigationSystem) == true)
		{
			FVector ActorLocation = ControlledPawn->GetActorLocation();
			FNavLocation NextLocation;
			if (NavigationSystem->GetRandomPointInNavigableRadius(ActorLocation, PatrolRadius, NextLocation) == true)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
			}
		}
	}
}
