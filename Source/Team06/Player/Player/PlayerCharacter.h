#pragma once

#include "PlayerBase.h"
#include "InputActionValue.h"
#include "Player/Component/GrabActor.h" 
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class USphereComponent;

UCLASS()
class TEAM06_API APlayerCharacter : public APlayerBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	//void Tick(float DeltaTime);

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

	void HandleFKey(const FInputActionValue& Value);

	void HandleCheatKey(const FInputActionValue& Value);
	void HandleCheat2Key(const FInputActionValue& Value);
	void HandleCheat3Key(const FInputActionValue& Value);
	void HandleCheat4Key(const FInputActionValue& Value);
	void HandleCheat5Key(const FInputActionValue& Value);

	void HandleGrabKey(const FInputActionValue& Value);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> FKeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> GrabAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> CheatKeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> Cheat2KeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> Cheat3KeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> Cheat4KeyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Input")
	TObjectPtr<UInputAction> Cheat5KeyAction;


#pragma endregion

#pragma region Attack
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SpawnProjectileFromItem();

	void CheckMeleeAttackHit(const FVector& AttackOffset);

	FVector GetPendingAttackOffset() const { return PendingAttackOffset; }
private:
	void HandleLeftHandMeleeAttack(const FInputActionValue& InValue);
	void HandleRightHandMeleeAttack(const FInputActionValue& InValue);
	void PerformMeleeAttack(const FVector& Offset, UAnimMontage* Montage, bool bIsLeftHand);
	void PerformRangedAttack(UAnimMontage* AttackMontage);
	void DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);
	void ResetLeftAttack();
	void ResetRightAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCLeftHandMeleeAttack(float InStartAttackTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCRightHandMeleeAttack(float InStartAttackTime);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayMeleeAttackMontage(UAnimMontage* AttackMontage);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastResetAttack(bool bIsLeftHand);

protected:
	//UFUNCTION()
	//void OnRep_CanAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCPerformMeleeHit(AActor* DamagedActor, float InCheckTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCItemMeleeAttack(float InStartTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCItemRangedAttack(float InStartTime);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> LeftMeleeAttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> RightMeleeAttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> ItemRightMeleeAttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> ItemRightRangedAttackMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Setting")
	TObjectPtr<UAnimMontage> ItemShortRangedAttackMontage = nullptr;

	//UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	UPROPERTY(Replicated)
	bool bLeftCanAttack = true;

	//UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	UPROPERTY(Replicated)
	bool bRightCanAttack = true;

	int32 isAlive = 0;

private:
	FTimerHandle AttackTimerHandle;
	FTimerHandle LeftAttackTimerHandle;

	FVector PendingAttackOffset;
	virtual void ServerProcessDeath(FVector RespawnLocation) override;
#pragma endregion

#pragma region Grab
	protected:
		UPROPERTY(EditDefaultsOnly, Category = "Grab")
		TSubclassOf<AGrabActor> GrabActorClass;

		UPROPERTY(VisibleAnywhere, Category = "Grab")
		USphereComponent* GrabSphere;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
		UPhysicsConstraintComponent* HandConstraint;

		UPROPERTY()
		AGrabActor* GrabActorInstance = nullptr;

		UFUNCTION(Server, Reliable)
		void ServerStartGrab();

		UFUNCTION(Server, Reliable)
		void ServerStopGrab();

		UFUNCTION(NetMulticast, Unreliable)
		void MulticastActivateGrab();

		UFUNCTION(NetMulticast, Unreliable)
		void MulticastReleaseGrab();

		void GrabTarget(USkeletalMeshComponent* TargetMesh, const FName& TargetBone);
#pragma endregion
};
