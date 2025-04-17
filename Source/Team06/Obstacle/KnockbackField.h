#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KnockbackField.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class APlayerBase;

UCLASS()
class TEAM06_API AKnockbackField : public AActor
{
	GENERATED_BODY()

public:
	AKnockbackField();

protected:
	virtual void BeginPlay() override;

	// 주기적으로 넉백 실행
	void TriggerKnockback();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayKnockbackEffects();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Knockback")
	bool bIsEnabled = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knockback")
	USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	float KnockbackRadius = 300.f;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	float KnockbackForce = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	float Interval = 5.f;

	UPROPERTY(EditAnywhere, Category = "Knockback|Effect")
	UNiagaraSystem* KnockbackEffect;

private:
	FTimerHandle KnockbackTimer;
};
