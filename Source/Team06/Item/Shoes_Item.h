#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "Shoes_Item.generated.h"

class APlayerBase;
class UAudioComponent;

UCLASS()
class TEAM06_API AShoes_Item : public ATriggerItem
{
    GENERATED_BODY()

public:
    AShoes_Item();

protected:
    virtual void BeginPlay() override;
    virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere, Category = "Effect")
    float SpeedBoost = 2000.0f; // 스피드 수치 조정 필요

    UPROPERTY(EditAnywhere, Category = "Effect")
    float JumpBoost = 2000.0f; // 점프 수치 조정 필요

    UPROPERTY(EditAnywhere, Category = "Effect")
    float Duration = 5.0f; // 지속 시간 수치 확인

    UPROPERTY(EditAnywhere, Category = "Effect|Sound")
    USoundBase* LoopSound;

    UPROPERTY()
    UAudioComponent* LoopSoundComp;

    bool bIsBuffActive = false;
    float StoredOrigSpeed = 0.f;
    float StoredOrigJump = 0.f;

private:
    FTimerHandle TimerHandle;

    void StopLoopSound();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ApplyBootsBuff(APlayerBase* Player, float NewSpeed, float NewJump);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_ResetBootsBuff(APlayerBase* Player, float OrigSpeed, float OrigJump);

    virtual void Multicast_PlayLoopEffect_Implementation(APlayerBase* Player) override;
    virtual void Multicast_StopLoopEffect_Implementation() override;
};