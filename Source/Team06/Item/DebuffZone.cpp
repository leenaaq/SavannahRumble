#include "DebuffZone.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ADebuffZone::ADebuffZone()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ADebuffZone::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	if (APlayerBase* Player = Cast<APlayerBase>(OverlappedActor))
	{
		if (HasAuthority())
		{
			UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
			if (MoveComp)
			{
				const float OriginalSpeed = MoveComp->MaxWalkSpeed;
				const float OriginalJump = MoveComp->JumpZVelocity;

				// 디버프 적용
				MoveComp->MaxWalkSpeed *= SpeedMultiplier;
				MoveComp->JumpZVelocity *= JumpMultiplier;

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Player, OriginalSpeed, OriginalJump]()
					{
						if (IsValid(Player))
						{
							UCharacterMovementComponent* MC = Player->GetCharacterMovement();
							if (MC)
							{
								MC->MaxWalkSpeed = OriginalSpeed;
								MC->JumpZVelocity = OriginalJump;
							}
						}
					}, DebuffDuration, false);
			}
		}

		if (UseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseSound, GetActorLocation());
		}

		Destroy();
	}
}
