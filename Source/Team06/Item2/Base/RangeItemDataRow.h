#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RangeItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FRangeItemDataRow : public FTableRowBase
{
	GENERATED_BODY()

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
};
