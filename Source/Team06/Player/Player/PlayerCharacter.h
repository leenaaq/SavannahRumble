#pragma once

#include "PlayerBase.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;

UCLASS()
class TEAM06_API APlayerCharacter : public APlayerBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

#pragma region Overrides Character

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void RecoverFromStun() override;

	virtual void OnStunned() override;

	void ValidateEssentialReferences();

#pragma endregion

#pragma region DXPlayerCharacter Components

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Components")
	TObjectPtr<UCameraComponent> Camera;

#pragma endregion

#pragma region Input

private:
	void HandleMoveInput(const FInputActionValue& InValue);

	void HandleLookInput(const FInputActionValue& InValue);

	void HandleBKey(const FInputActionValue& Value);

	void HandleESCKey(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> LeftHandAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> RightHandAttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> BKeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> ESCKeyAction;

#pragma endregion

#pragma region Attack
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void HandleLeftHandMeleeAttack(const FInputActionValue& InValue);
	void HandleRightHandMeleeAttack(const FInputActionValue& InValue);
	void PerformMeleeAttack(const FVector& Offset, UAnimMontage* AttackMontage);
	void CheckMeleeAttackHit(const FVector& AttackOffset);
	void DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);
	void ResetAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCLeftHandMeleeAttack(float InStartAttackTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCRightHandMeleeAttack(float InStartAttackTime);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMeleeAttackMontage(UAnimMontage* AttackMontage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastResetAttack();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> LeftMeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> RightMeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> ItemRightMeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> ItemRightRangedAttackMontage;

	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	bool bCanAttack;

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	void OnRep_CanAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCPerformMeleeHit(AActor* DamagedActor, float InCheckTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCItemMeleeAttack(float InStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCItemRangedAttack(float InStartTime);


#pragma endregion
};
