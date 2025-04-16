#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/Component/PlayerTextWidgetComponent.h"
#include "Engine/DataTable.h"
#include "Components/ArrowComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "PlayerBase.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHigh = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KnockBack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LifeCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StunDuration = 0.0f;


};

UCLASS()
class TEAM06_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerBase();

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerProcessDeath(FVector RespawnLocation);

	UFUNCTION(Server, Reliable)
	void RespawnCharacter(FVector RespawnLocation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetEquippedItemName(FName NewItemName);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ValidateEssentialReferences();
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// 데이터 테이블 적용
	void UpdateStatsFromDataTable();

	// 기절
	UFUNCTION(BlueprintCallable, Category = "Player")
	virtual void OnStunned();
	virtual void OnStunned(float StunTime);

	// Getters
	float GetHealth() const { return PlayerStats.Health; }
	float GetMaxHealth() const { return PlayerStats.MaxHealth; }
	float GetAttackDamage() const { return PlayerStats.AttackDamage; }
	float GetWalkSpeed() const { return PlayerStats.WalkSpeed; }
	float GetJumpHigh() const { return PlayerStats.JumpHigh; }
	int32 GetKnockBack() const { return PlayerStats.KnockBack; }
	int32 GetLifeCount() const { return PlayerStats.LifeCount; }
	bool GetbIsStunned() const { return bIsStunned; }
	FName GetCurrentEquippedItemName() const { return CurrentEquippedItemName; }
	const FPlayerStats& GetStats() const { return PlayerStats; }
	bool GetbHasFlag() const { return bHasFlag; }

	// Setters
	void SetHealth(float NewHealth) { PlayerStats.Health = NewHealth; }
	void SetMaxHealth(float NewMaxHealth) { PlayerStats.MaxHealth = NewMaxHealth; }
	void SetAttackDamage(float NewAttackDamage) { PlayerStats.AttackDamage = NewAttackDamage; }
	void SetWalkSpeed(float NewWalkSpeed) { PlayerStats.WalkSpeed = NewWalkSpeed; }
	void SetJumpHigh(float NewJumpHigh) { PlayerStats.JumpHigh = NewJumpHigh; }
	void SetKnockBack(int32 NewKnockBack) { PlayerStats.KnockBack = NewKnockBack; }
	void SetLifeCount(int32 NewLifeCount) { PlayerStats.LifeCount = NewLifeCount; }
	void SetbIsStunned(bool NewbIsStunned) { bIsStunned = NewbIsStunned; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetEquippedItemName(FName NewItemName) { CurrentEquippedItemName = NewItemName; }
	void SetbHasFlag(bool NewbHasFlag) { bHasFlag = NewbHasFlag; }
	//void SetEquipItemMeshStatic(UStaticMesh* NewMesh);

	UFUNCTION()
	void OnRep_CurrentEquippedItemName();

protected:
	UFUNCTION()
	void OnRep_bIsStunned();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRecoverFromStun();

	virtual void RecoverFromStun();

	UFUNCTION()
	void OnRep_SkinName();

	void UpdateSkinVisibility();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSkinName(FName NewSkinName);

	UFUNCTION(BlueprintCallable, Category = "Character|Skin")
	void SetSkinName(FName NewSkinName);
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle StatsRowHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting|Component")
	UChildActorComponent* EquipItemMeshActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Mesh")
	USkeletalMeshComponent* PigMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Mesh")
	USkeletalMeshComponent* FoxMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Mesh")
	USkeletalMeshComponent* WolfMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Mesh")
	USkeletalMeshComponent* DeerMeshComponent;

	UPROPERTY(ReplicatedUsing = OnRep_SkinName, BlueprintReadOnly, Category = "Character|Skin")
	FName SkinName = "Pig";

	void ActiveRagdoll();
	void DeactivateActiveRagdoll();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UItemManagerComponent* ItemManager;

	UPROPERTY(ReplicatedUsing = OnRep_bIsStunned, BlueprintReadOnly)
	bool bIsStunned = false;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquippedItemName, BlueprintReadWrite)
	FName CurrentEquippedItemName = "DEFAULT";

	UPROPERTY(Replicated, BlueprintReadOnly)
	float RemainingStunTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* MuzzleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicalAnimationComponent* PhysicalAnimationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerTextWidgetComponent> PlayerNameWidgetComponent;

private:
	UPROPERTY(Replicated)
	FPlayerStats PlayerStats;

	FTimerHandle RecoveryTimerHandle;
	FPhysicalAnimationData PhysAnimData;
	FTimerHandle RecoveryRagdollTimerHandle;
	bool bPlayerNameInitialized = false;
	bool bHasFlag = false;

};