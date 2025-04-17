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
    APlayerCharacter* OwnerPC = Cast<APlayerCharacter>(OwnerCharacter);
    if (!IsValid(OwnerPC))
        return;

    if (OwnerPC->IsLocallyControlled())
    {
        OwnerPC->ServerSpawnProjectileFromItem(1.f, 1.f);
    }

    FTimerHandle TempHandle;
    if (APlayerController* PC = Cast<APlayerController>(OwnerPC->GetController()))
    {
        PC->GetWorldTimerManager().SetTimer(
            TempHandle,
            [OwnerPC]()
            {
                if (IsValid(OwnerPC))
                {
                    OwnerPC->ServerSetEquippedItemName("DEFAULT");
                }
            },
            0.1f,
            false
        );
    }
}

void UPlayerAnimInstanceBase::AnimNotify_CheckShortRangedAttack()
{
    APlayerCharacter* OwnerPC = Cast<APlayerCharacter>(OwnerCharacter);
    if (!IsValid(OwnerPC))
        return;

    if (OwnerPC->IsLocallyControlled())
    {
        OwnerPC->ServerSpawnProjectileFromItem(.9f, .9f);
    }

    FTimerHandle TempHandle;
    if (APlayerController* PC = Cast<APlayerController>(OwnerPC->GetController()))
    {
        PC->GetWorldTimerManager().SetTimer(
            TempHandle,
            [OwnerPC]()
            {
                if (IsValid(OwnerPC))
                {
                    OwnerPC->ServerSetEquippedItemName("DEFAULT");
                }
            },
            0.1f,
            false
        );
    }
}


void UPlayerAnimInstanceBase::AnimNotify_CheckGetupEnd()
{
    AAICharacter* AI = Cast<AAICharacter>(OwnerCharacter);
    if (IsValid(AI))
    {
        if (AI->HasAuthority())
        {
            AI->AIMovementOn();
        }
    }
    APlayerCharacter* Player = Cast<APlayerCharacter>(OwnerCharacter);
    if (IsValid(Player))
    {
        if (Player->HasAuthority())
        {
            Player->CharacterMovementOn();
        }
    }
}