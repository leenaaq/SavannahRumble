#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class UNiagaraSystem;
class USoundBase;
class UParticleSystem;

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

	// 플레이 함수
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayItemEffects(FVector Location);

};
