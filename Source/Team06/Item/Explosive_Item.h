#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Explosive_Item.generated.h"


UCLASS()
class TEAM06_API AExplosive_Item : public AItem
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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* ExplosionArea;

	UFUNCTION()
	void Explode();

private:
	FTimerHandle ExplosionTimerHandle;
};
