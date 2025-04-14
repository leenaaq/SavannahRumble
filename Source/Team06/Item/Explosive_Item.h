#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Explosive_Item.generated.h"

class USphereComponent;

UCLASS()
class TEAM06_API AExplosive_Item : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AExplosive_Item();

protected:
	virtual void BeginPlay() override;

	virtual void OnItemLanded_Implementation() override; // 착지 후 폭발 예약|

	UFUNCTION()
	void Explode();

	/** 클라이언트에 폭발 이펙트 보여주기 */
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ExplodeEffect();

	// 폭발 기능
	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionRadius = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float Damage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float KnockbackStrength = 1200.0f;

	// 폭발 범위 콜리전만 특화로 유지
	UPROPERTY(VisibleAnywhere, Category = "Explosion")
	USphereComponent* ExplosionArea;

	UPROPERTY(EditAnywhere, Category = "Explosion|Effect")
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "Explosion|Sound")
	USoundBase* ExplosionSound;


private:
	FTimerHandle ExplosionTimerHandle;
};
