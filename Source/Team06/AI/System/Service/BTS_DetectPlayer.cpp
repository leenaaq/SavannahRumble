// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Service/BTS_DetectPlayer.h"
#include "AI/Character/AICharacter.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Player/PlayerCharacter.h"
#include "Engine/OverlapResult.h"

UBTS_DetectPlayer::UBTS_DetectPlayer(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("DetectPlayerCharacter");
	Interval = 1.f;
}

void UBTS_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIC_Enemy* AIC = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIC) == true)
	{
		AAICharacter* NPC = Cast<AAICharacter>(AIC->GetPawn());
		if (IsValid(NPC) == true)
		{
			UWorld* World = NPC->GetWorld();
			if (IsValid(World) == true)
			{
				FVector DetectCenter = NPC->GetActorLocation();
				float DetectRadius = 500.f;
				TArray<FOverlapResult> OverlapResults;
				FCollisionQueryParams CollisionQueryParams(NAME_None, false, NPC);
				bool bResult = World->OverlapMultiByChannel(
					OverlapResults,
					DetectCenter,
					FQuat::Identity,
					ECollisionChannel::ECC_GameTraceChannel2,
					FCollisionShape::MakeSphere(DetectRadius),
					CollisionQueryParams
				);

				if (bResult == true)
				{
					for (auto const& OverlapResult : OverlapResults)
					{

						APlayerCharacter* PC = Cast<APlayerCharacter>(OverlapResult.GetActor());
						if (IsValid(PC) == true && PC->GetController()->IsPlayerController() == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::OtherPlayerLocationKey, PC);

							if (AAIC_Enemy::ShowAIDebug == 1)
							{
								UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("AI Detecting Player!")));
								DrawDebugSphere(World, DetectCenter, DetectRadius, 16, FColor::Red, false, 0.5f);
								DrawDebugPoint(World, PC->GetActorLocation(), 10.f, FColor::Red, false, 0.5f);
								DrawDebugLine(World, NPC->GetActorLocation(), PC->GetActorLocation(), FColor::Red, false, 0.5f, 0u, 3.f);
							}

							break;
						}
						else
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::OtherPlayerLocationKey, nullptr);

							if (AAIC_Enemy::ShowAIDebug == 1)
							{
								DrawDebugSphere(World, DetectCenter, DetectRadius, 16, FColor::Green, false, 0.5f);
							}
						}
					}
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AAIC_Enemy::OtherPlayerLocationKey, nullptr);
				}

				if (AAIC_Enemy::ShowAIDebug == 1)
				{
					DrawDebugSphere(World, DetectCenter, DetectRadius, 16, FColor::Green, false, 0.5f);
				}
			}
		}
	}
}
