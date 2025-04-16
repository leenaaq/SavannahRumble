// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/System/Tasks/BTT_AI_Jumping.h"
#include "AI/System/AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Character/AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_AI_Jumping::UBTT_AI_Jumping()
{
	NodeName = TEXT("AI_MeleeAttack");
}

EBTNodeResult::Type UBTT_AI_Jumping::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	AAIC_Enemy* AIController = Cast<AAIC_Enemy>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		AAICharacter* AICharacter = Cast<AAICharacter>(AIController->GetPawn());
		if (IsValid(AICharacter))
		{
			UCharacterMovementComponent* MovementComp = AICharacter->GetCharacterMovement();

			if (MovementComp && !MovementComp->IsFalling())
			{
				const FVector LaunchVelocity(0.0f, 0.0f, 600.0f);
				AICharacter->LaunchCharacter(LaunchVelocity, false, true);

				return EBTNodeResult::Succeeded;
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}

		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
