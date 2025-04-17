#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item2Base.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

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

    UFUNCTION(NetMulticast, Reliable)
    void OnRespawned();

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

public:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ParticleEffect1;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ParticleEffect2;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* ParticleEffect3;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffect1;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffect2;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraEffect3;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* SoundEffect1;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* SoundEffect2;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* SoundEffect3;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateParticle1();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateParticle2();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateParticle3();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateNiagara1();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateNiagara2();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ActivateNiagara3();

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySound1();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySound2();
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySound3();

	UFUNCTION(Server, Reliable)
	void ServerActivateParticle1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateParticle1();

	UFUNCTION(Server, Reliable)
	void ServerActivateParticle2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateParticle2();

	UFUNCTION(Server, Reliable)
	void ServerActivateParticle3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateParticle3();

	UFUNCTION(Server, Reliable)
	void ServerActivateNiagara1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateNiagara1();

	UFUNCTION(Server, Reliable)
	void ServerActivateNiagara2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateNiagara2();

	UFUNCTION(Server, Reliable)
	void ServerActivateNiagara3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateNiagara3();

	UFUNCTION(Server, Reliable)
	void ServerPlaySound1();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound1();

	UFUNCTION(Server, Reliable)
	void ServerPlaySound2();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound2();

	UFUNCTION(Server, Reliable)
	void ServerPlaySound3();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySound3();
};