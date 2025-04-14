#include "Item/ItemBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Player/PlayerBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::PlayItemEffects(FVector Location)
{
	if (NiagaraEffect)
	{
		SpawnedNiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Location);
	}
	if (LegacyEffect)
	{
		SpawnedLegacyEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LegacyEffect, Location);
	}
	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseSound, Location);
	}
}

// 이펙트 제거 함수 추가
void AItemBase::StopItemEffects()
{
	if (SpawnedNiagaraEffect)
	{
		SpawnedNiagaraEffect->Deactivate();
		SpawnedNiagaraEffect = nullptr;
	}
	if (SpawnedLegacyEffect)
	{
		SpawnedLegacyEffect->DeactivateSystem();
		SpawnedLegacyEffect = nullptr;
	}
}

void AItemBase::Multicast_PlayLoopEffect_Implementation(APlayerBase* Target)
{
	if (!Target || !NiagaraEffect) return;

	SpawnedNiagaraEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraEffect,
		Target->GetRootComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true, true
	);
}

// 기본 캐릭터 적용 처리 함수 (자식에서 오버라이드 가능)
void AItemBase::ApplyToCharacter_Implementation(APlayerBase* Character)
{
	UE_LOG(LogTemp, Log, TEXT("[ItemBase] %s 캐릭터에게 아이템 적용됨"), *Character->GetName());
}

void AItemBase::Multicast_StopLoopEffect_Implementation()
{
	if (ActiveEffectComp)
	{
		ActiveEffectComp->Deactivate();
		ActiveEffectComp = nullptr;

		UE_LOG(LogTemp, Log, TEXT("[ItemBase] 루프 이펙트 중단 (Multicast)"));
	}
}