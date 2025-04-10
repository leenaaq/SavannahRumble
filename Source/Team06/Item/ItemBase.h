#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UParticleSystem;
class UParticleSystemComponent;
class USoundBase;
class APlayerBase;

UCLASS()
class TEAM06_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItemBase();

protected:
	
	virtual void BeginPlay() override;
	
	// 이펙트 & 사운드 공통 속성
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* LegacyEffect;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* UseSound;

	// 루프 이펙트 (플레이어 몸에 부착)
	UPROPERTY(EditAnywhere, Category = "Effect|Loop")
	UNiagaraSystem* LoopEffect;

	UPROPERTY()
	UNiagaraComponent* ActiveEffectComp;

	// 사운드 재생
	UPROPERTY()
	UParticleSystemComponent* SpawnedLegacyEffect;

	// 이펙트/사운드 실행
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayItemEffects(FVector Location);

	// 이펙트 제거 함수
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StopItemEffects();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayLoopEffect(APlayerBase* Target);

	// ===== 멀티캐스트 함수 (모든 클라이언트에서 제거) =====
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopLoopEffect();

	// 상호작용 캐릭터 처리용 함수 (오버라이딩 가능)
	UFUNCTION(BlueprintNativeEvent, Category = "Item|Interaction")
	void ApplyToCharacter(APlayerBase* Character);
	virtual void ApplyToCharacter_Implementation(APlayerBase* Character);
};
