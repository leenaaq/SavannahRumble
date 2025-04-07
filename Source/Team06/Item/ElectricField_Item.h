#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "ElectricField_Item.generated.h"

UCLASS()
class TEAM06_API AElectricField_Item : public AEquipableItem
{
	GENERATED_BODY()
	
public:
	AElectricField_Item();

protected:
	virtual void BeginPlay() override;

	// 아이템 착지 후 전기장 발동
	void ActivateElectricField();

	// 자기장 제거
	UFUNCTION()
	void DestroySelf();

	// 클라 동기화를 위한 RPC
	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateField();

	//충돌 감지 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNiagaraComponent* ElectricEffect;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* FieldArea;

	UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
	float ActivationDelay = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "ElectricEffect")
	float FieldDuration = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "ElectricEffect")
	float RagdollTime = 2.0f;

	FTimerHandle ActivationTimer;
	FTimerHandle DestroyTimer;
};

