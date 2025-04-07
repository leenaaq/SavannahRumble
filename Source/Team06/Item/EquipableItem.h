#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipableItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TEAM06_API AEquipableItem : public AActor
{
	GENERATED_BODY()

public:
	AEquipableItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//아이템 메시 (공통 시각적 표현)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	//충돌 범위 (공통 물리/충돌)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

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

