#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class TEAM06_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyDamageAndKnockback(AActor* Target, float Damage, float KnockbackForce);
	virtual void ServerApplyDamageAndKnockback_Implementation(AActor* Target, float Damage, float KnockbackForce);
	virtual bool ServerApplyDamageAndKnockback_Validate(AActor* Target, float Damage, float KnockbackForce);

	// 아이템 사용 함수 (모든 아이템에서 사용 가능)
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerUseItem(AActor* Target);
	virtual void ServerUseItem_Implementation(AActor* Target);
	virtual bool ServerUseItem_Validate(AActor* Target) { return true; }

	// 아이템 던지기 (모든 아이템에서 사용 가능)
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerThrowItem(FVector Direction);
	virtual void ServerThrowItem_Implementation(FVector Direction);
	virtual bool ServerThrowItem_Validate(FVector Direction) { return true; }

	// 충돌 처리 (모든 아이템에서 사용 가능)
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};

