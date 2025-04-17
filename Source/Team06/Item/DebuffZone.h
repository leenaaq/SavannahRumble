#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "DebuffZone.generated.h"

class UStaticMeshComponent;
class USoundBase;

UCLASS()
class TEAM06_API ADebuffZone : public ATriggerItem
{
	GENERATED_BODY()

public:
	ADebuffZone();

protected:
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;

	//디버프 관련 설정값
	UPROPERTY(EditAnywhere, Category = "Debuff")
	float SpeedMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float JumpMultiplier = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Debuff")
	float DebuffDuration = 3.0f;
};
