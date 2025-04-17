#pragma once

#include "CoreMinimal.h"
#include "Item/TriggerItem.h"
#include "Sound/SoundBase.h"
#include "Shoes_Item.generated.h"

UCLASS()
class TEAM06_API AShoes_Item : public ATriggerItem
{
    GENERATED_BODY()

public:
    AShoes_Item();

protected:
    virtual void BeginPlay() override;
    virtual void TriggerEffect_Implementation(AActor* OverlappedActor) override;

    UPROPERTY(EditAnywhere, Category = "Effect")
    float SpeedBoost = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Effect")
    float JumpBoost = 2000.0f;

    UPROPERTY(EditAnywhere, Category = "Effect")
    float Duration = 5.0f; // 버프 지속 시간

    // ------ 이펙트 & 사운드 ------

        // 루프되는 사운드 (효과 지속 중)
    UPROPERTY(EditAnywhere, Category = "Effect|Sound")
    USoundBase* LoopSound;

    // 사운드 컴포넌트 (실행 중 사운드 제어용)
    UPROPERTY()
    UAudioComponent* LoopSoundComp;

    FTimerHandle TimerHandle;

    // 루프 사운드 중지
    void StopLoopSound();

    void EndPlay(const EEndPlayReason::Type EndPlayReason);
};