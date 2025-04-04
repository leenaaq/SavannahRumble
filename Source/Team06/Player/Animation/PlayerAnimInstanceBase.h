#pragma once

#include "Animation/AnimInstance.h"
#include "PlayerAnimInstanceBase.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 *
 */
UCLASS()
class TEAM06_API UPlayerAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	uint8 bShouldMove : 1;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;

};
