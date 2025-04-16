// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Player/PlayerBase.h"
#include "AICharacter.generated.h"

class USphereComponent;
class AFlagActor;

/**
 * 
 */
UCLASS()
class TEAM06_API AAICharacter : public APlayerBase
{
	GENERATED_BODY()
	
public:
	AAICharacter();

	//virtual void Tick(float DeltaTime) override;

	virtual void OnStunned();
	virtual void OnStunned(float StunTime);
	virtual void RecoverFromStun();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	// ---------- Add Components ----------
protected:
	USphereComponent* DetectingFlagCollision;

	// ---------- Proerty Declaration ----------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectorRadius;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName = "spine_03";

	// ---------- Other Property ----------
private:
	AFlagActor* MyFlag = nullptr;

	bool bHasAnyEquip = false;

	// ---------- Like Player ----------
#pragma region Attack
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void SpawnProjectileFromItem();

	void CheckMeleeAttackHit(const FVector& AttackOffset);

	FVector GetPendingAttackOffset() const { return PendingAttackOffset; }
private:
	void HandleLeftHandMeleeAttack();
	void HandleRightHandMeleeAttack();
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


};
