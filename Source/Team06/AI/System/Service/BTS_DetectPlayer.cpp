// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Service/BTS_DetectPlayer.h"
#include "AI/Character/AICharacter.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Player/PlayerCharacter.h"
#include "Engine/OverlapResult.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UBTS_DetectPlayer::UBTS_DetectPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("DetectPlayerCharacter");
	Interval = 0.15f;
}

void UBTS_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), Players);

		if (Players.Num() > 0)
		{
			float NearsetActorDistance = 100.f;
			AActor* NearsetPlayer = UGameplayStatics::FindNearestActor(OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(), Players, NearsetActorDistance);

			AIController->SetTargetPlayer(NearsetPlayer);
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::ClosePlayerKey, NearsetPlayer);
		}
	}
}
