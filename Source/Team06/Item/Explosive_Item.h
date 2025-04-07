#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Explosive_Item.generated.h"


UCLASS()
class TEAM06_API AExplosive_Item : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AExplosive_Item();

protected:
	virtual void BeginPlay() override;

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

	UFUNCTION()
	void Explode();

private:
	FTimerHandle ExplosionTimerHandle;
};
