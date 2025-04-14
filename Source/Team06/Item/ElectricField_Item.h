#pragma once

#include "CoreMinimal.h"
#include "Item/EquipableItem.h"
#include "ElectricField_Item.generated.h"

class UNiagaraSystem;
class USphereComponent;

UCLASS()
class TEAM06_API AElectricField_Item : public AEquipableItem
{
    GENERATED_BODY()

public:
    AElectricField_Item();

protected:
    virtual void BeginPlay() override;

public:
    virtual void OnItemPickedUp(AActor* OtherActor);
    virtual void ServerUseItem_Implementation(AActor* Target) override;

    void ActivateElectricField(); // 서버에서 실행됨
    void ApplyFieldEffect();      // 실제 효과 적용
    void DestroyItem();           // 아이템 제거

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_PlayFieldEffect();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
    float FieldRadius = 400.f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
    float FieldDuration = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
    float ActivationDelay = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
    float RagdollTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ElectricField")
    UNiagaraSystem* FieldEffect;

    FTimerHandle ActivationTimerHandle;
    FTimerHandle DurationTimerHandle;

    bool bFieldActivated = false;
};
