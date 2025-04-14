#include "Item/Explosive_Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AExplosive_Item::AExplosive_Item()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// 폭발 영역 콜리전 추가
	ExplosionArea = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionArea"));
	ExplosionArea->SetupAttachment(RootComponent);
	ExplosionArea->SetSphereRadius(ExplosionRadius);
	ExplosionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExplosionArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	ExplosionArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 생성됨 (Constructor)"));
}

void AExplosive_Item::BeginPlay()
{
	Super::BeginPlay();

}

void AExplosive_Item::OnItemLanded_Implementation()
{
	Super::OnItemLanded_Implementation();

	if (!HasAuthority() || bIsActivated) return;

	bIsActivated = true;

	ExplosionArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UE_LOG(LogTemp, Log, TEXT("[Explosive_Item] 착지 감지됨 → %.1f초 후 폭발 예정"), ExplosionDelay);

	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive_Item::Explode, ExplosionDelay, false);
}

void AExplosive_Item::Explode()
{
	if (!HasAuthority()) return;

	Multicast_ExplodeEffect();

	TArray<AActor*> OverlappedActors;
	ExplosionArea->GetOverlappingActors(OverlappedActors, ACharacter::StaticClass());

	for (AActor* Actor : OverlappedActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			FVector KnockDir = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector Force = KnockDir * KnockbackStrength;

			Character->LaunchCharacter(Force, true, true);
			UGameplayStatics::ApplyDamage(Character, Damage, GetInstigatorController(), this, nullptr);

			UE_LOG(LogTemp, Log, TEXT("[Explosive_Item] %s 넉백 및 데미지 %.0f 적용됨"), *Character->GetName(), Damage);
		}
	}

	Destroy();
}

void AExplosive_Item::Multicast_ExplodeEffect_Implementation()
{
	// 이펙트
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}
	// 사운드
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 16, FColor::Red, false, 2.0f);
}