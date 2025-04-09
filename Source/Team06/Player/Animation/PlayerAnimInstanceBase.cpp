#include "PlayerAnimInstanceBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Player/PlayerCharacter.h"

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
	if (IsValid(OwnerPlayerCharacter) == true)
	{
		//OwnerPlayerCharacter->CheckMeleeAttackHit(AttackOffset);
	}
}

void UPlayerAnimInstanceBase::AnimNotify_CheckRangedAttack()
{
	APlayerCharacter* OwnerPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
	if (IsValid(OwnerPlayerCharacter) == true)
	{
		OwnerPlayerCharacter->SpawnProjectileFromItem();
	}
}
