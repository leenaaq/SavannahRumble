#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item2Base.generated.h"

UENUM(BlueprintType)
enum class EItemState : uint8
{
	Flying UMETA(DisplayName = "Flying"),
	Landed UMETA(DisplayName = "Landed")
};

UCLASS()
class TEAM06_API AItem2Base : public AActor
{
	GENERATED_BODY()

public:
	AItem2Base();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnSpawn();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void OnCollision(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void TransitionToLanded();

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void ApplyEffect(class APlayerBase* AffectedPlayer);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void ServerHandleCollision(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void InitializeFromDataTable();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float StunDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float ExplosionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float KnockBackMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UDataTable* ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FName ItemRowName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	float SpawnTime;
};
