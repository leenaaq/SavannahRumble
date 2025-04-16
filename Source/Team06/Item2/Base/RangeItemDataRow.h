#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RangeItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FRangeItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

	//-----------------------폭탄---------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float StunDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float ExplosionDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float ExplosionRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float KnockBackMultiplier = 0.0f;

	//-----------------------자기장---------------------

	// [자기장] 초당 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float DamagePerSecond = 0.0f;

	// [자기장] 원 최대 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float MaxRadius = 0.0f;

	// [자기장] 최대 반경까지 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float ExpansionTime = 0.0f;

	//-----------------------끈끈이---------------------

	// [끈끈이] 끈끈이 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float SlowDuration = 0.0f;

	// [끈끈이] 속도 감소 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float SpeedMultiplier = 1.0f;

	// [끈끈이] 점프력 감소 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Balance")
	float JumpMultiplier = 1.0f;

};
