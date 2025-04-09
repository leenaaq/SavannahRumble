#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "GameFramework/Actor.h"
#include "EquipableItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TEAM06_API AEquipableItem : public AItemBase
{
	GENERATED_BODY()

public:
	AEquipableItem();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComp;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyDamageAndKnockback(AActor* Target, float Damage, float KnockbackForce);
	virtual void ServerApplyDamageAndKnockback_Implementation(AActor* Target, float Damage, float KnockbackForce);
	virtual bool ServerApplyDamageAndKnockback_Validate(AActor* Target, float Damage, float KnockbackForce);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerUseItem(AActor* Target);
	virtual void ServerUseItem_Implementation(AActor* Target);
	virtual bool ServerUseItem_Validate(AActor* Target) { return true; }

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerThrowItem(FVector Direction);
	virtual void ServerThrowItem_Implementation(FVector Direction);
	virtual bool ServerThrowItem_Validate(FVector Direction) { return true; }

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, Category = "Equipable|Activation")
	void OnItemLanded();
	virtual void OnItemLanded_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Equipable|Activation")
	void ActivateEffect();
	virtual void ActivateEffect_Implementation();

	UFUNCTION()
	void HandleImpact(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Equipable|Activation")
	float ActivationDelayAfterLanding = 2.0f;

	UPROPERTY(Replicated)
	bool bHasLanded = false;

	UPROPERTY(Replicated)
	bool bIsActivated = false;

	UPROPERTY(Replicated)
	bool bWasThrown = false;

	FTimerHandle ActivationTimerHandle;
};
