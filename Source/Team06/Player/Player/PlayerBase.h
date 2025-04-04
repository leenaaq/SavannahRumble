#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "PlayerBase.generated.h"

//USTRUCT(BlueprintType)
//struct FPlayerStats : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float Health;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float AttackDamage;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float WalkSpeed;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float JumpHigh;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 KnockBack;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 LifeCount;
//};

UCLASS()
class TEAM06_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerBase();

//	// Functions to update stats from DataTable
//	void UpdateStatsFromDataTable();
//
//	// Getters
//	float GetHealth() const { return PlayerStats.Health; }
//	float GetAttackDamage() const { return PlayerStats.AttackDamage; }
//	float GetWalkSpeed() const { return PlayerStats.WalkSpeed; }
//	float GetJumpHigh() const { return PlayerStats.JumpHigh; }
//	int32 GetKnockBack() const { return PlayerStats.KnockBack; }
//	int32 GetLifeCount() const { return PlayerStats.LifeCount; }
//
//	// Setters
//	void SetHealth(float NewHealth) { PlayerStats.Health = NewHealth; }
//	void SetAttackDamage(float NewAttackDamage) { PlayerStats.AttackDamage = NewAttackDamage; }
//	void SetWalkSpeed(float NewWalkSpeed) { PlayerStats.WalkSpeed = NewWalkSpeed; }
//	void SetJumpHigh(float NewJumpHigh) { PlayerStats.JumpHigh = NewJumpHigh; }
//	void SetKnockBack(int32 NewKnockBack) { PlayerStats.KnockBack = NewKnockBack; }
//	void SetLifeCount(int32 NewLifeCount) { PlayerStats.LifeCount = NewLifeCount; }
//
//	// DataTable row handle
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
//	FDataTableRowHandle StatsRowHandle;
//
//private:
//	FPlayerStats PlayerStats;
};