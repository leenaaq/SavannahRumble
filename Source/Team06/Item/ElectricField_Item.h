#pragma once

#include "CoreMinimal.h"
#include "Item/EquipableItem.h"
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
    virtual void OnItemLanded_Implementation() override; // 착지 후 발동 오버라이드
    virtual void ServerUseItem_Implementation(AActor* Target) override;

    /** 자기장 발동 */
    void ActivateElectricField();

    /** 자기장 제거 */
    void DestroySelf();

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* FieldArea;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricEffect")
    float FieldDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricEffect")
    float RagdollTime = 2.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricEffect")
    UNiagaraSystem* FieldEffect; // Niagara 이펙트
};


