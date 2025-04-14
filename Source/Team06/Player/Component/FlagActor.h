#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagActor.generated.h"

class USphereComponent;
class UCapsuleComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EFlagState : uint8
{
	Idle,
	Active
};

UCLASS()
class TEAM06_API AFlagActor : public AActor
{
	GENERATED_BODY()

public:
	AFlagActor();

protected:
	virtual void BeginPlay() override;
	void SetFlagState(EFlagState NewState);
	void AttachToPlayer(AActor* NewOwner);

	UFUNCTION()
	void OnIdleTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnGameTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ResetSwapCooldown();

protected:

	EFlagState FlagState = EFlagState::Idle;
	FString CurrentOwnerName;
	bool bCanSwap = true;
	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* IdleTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* GameTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* FlagMesh;

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocketName = "spine_03";
};
