#include "PlayerAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Player/PlayerCharacter.h"
#include "AI/Character/AICharacter.h"

void UPlayerAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwnerCharacter) == true)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerCharacter) == false || IsValid(OwnerCharacterMovementComponent) == false)
	{
		return;
	}

	Velocity = OwnerCharacterMovementComponent->Velocity;
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = ((OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero()) == false) && (3.f < GroundSpeed);
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
}

void UPlayerAnimInstanceBase::AnimNotify_CheckMeleeAttackHit()
{
    APlayerCharacter* OwnerPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
    if (IsValid(OwnerPlayerCharacter))
    {
        if (OwnerPlayerCharacter->HasAuthority())
        {
            OwnerPlayerCharacter->CheckMeleeAttackHit(OwnerPlayerCharacter->GetPendingAttackOffset());
        }

        //UAnimMontage* CurrentMontage = GetCurrentActiveMontage();
        //if (IsValid(CurrentMontage))
        //{
        //    Montage_SetPlayRate(CurrentMontage, 0.f);
        //}
    }

	AAICharacter* AI = Cast<AAICharacter>(OwnerCharacter);
	if (IsValid(AI))
	{
		if (AI->HasAuthority())
		{
			AI->CheckMeleeAttackHit(AI->GetPendingAttackOffset());
		}
	}
}

void UPlayerAnimInstanceBase::AnimNotify_CheckRangedAttack()
{
    APlayerCharacter* OwnerPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
    if (IsValid(OwnerPlayerCharacter))
    {
        OwnerPlayerCharacter->SpawnProjectileFromItem();

        FTimerHandle TempHandle;
        APlayerController* PC = Cast<APlayerController>(OwnerPlayerCharacter->GetController());
        if (IsValid(PC))
        {
            PC->GetWorldTimerManager().SetTimer(
                TempHandle,
                [OwnerPlayerCharacter]()
                {
                    if (IsValid(OwnerPlayerCharacter))
                    {
                        OwnerPlayerCharacter->ServerSetEquippedItemName("DEFAULT");
                    }
                },
                0.1f,
                false
            );
        }
    }
}
