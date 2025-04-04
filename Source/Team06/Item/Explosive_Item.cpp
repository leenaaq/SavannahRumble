#include "Explosive_Item.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

AExplosive_Item::AExplosive_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosionArea = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionArea"));
	ExplosionArea->SetupAttachment(RootComponent);
	ExplosionArea->SetSphereRadius(ExplosionRadius);
	ExplosionArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 필요 없음

	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 생성됨 (Constructor)"));
}

void AExplosive_Item::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] BeginPlay 호출됨. %.1f초 뒤에 폭발 예정"), ExplosionDelay);

	// 일정 시간 후 폭발 예약
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive_Item::Explode, ExplosionDelay, false);
}

void AExplosive_Item::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 폭발 발생! 데미지: %.1f, 반경: %.1f"), Damage, ExplosionRadius);

	// 데미지 적용
	UGameplayStatics::ApplyRadialDamage(
		this,
		Damage,
		GetActorLocation(),
		ExplosionRadius,
		nullptr,
		TArray<AActor*>(), // 무시할 액터 없음
		this,
		GetInstigatorController(),
		true
	);

	// 넉백 효과
	TArray<AActor*> OverlappedActors;

	UKismetSystemLibrary::SphereOverlapActors(
		this,
		GetActorLocation(),
		ExplosionRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>> { EObjectTypeQuery::ObjectTypeQuery3 }, // Pawn
		ACharacter::StaticClass(),
		TArray<AActor*>(), // 무시 목록
		OverlappedActors
	);

	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 넉백 대상 %d명 발견"), OverlappedActors.Num());

	for (AActor* Actor : OverlappedActors)
	{
		ACharacter* AffectedChar = Cast<ACharacter>(Actor);
		if (AffectedChar)
		{
			FVector KnockbackDir = AffectedChar->GetActorLocation() - GetActorLocation();
			KnockbackDir.Normalize();

			UCharacterMovementComponent* MoveComp = AffectedChar->GetCharacterMovement();
			if (MoveComp)
			{
				MoveComp->AddImpulse(KnockbackDir * KnockbackStrength, true);
				UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] %s 에게 넉백 적용됨"), *AffectedChar->GetName());
			}
		}
	}

	Destroy(); // 폭탄 제거
	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 폭탄 제거됨"));
}