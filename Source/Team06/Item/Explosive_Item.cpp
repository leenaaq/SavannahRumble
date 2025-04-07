#include "Explosive_Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

AExplosive_Item::AExplosive_Item()
{
	PrimaryActorTick.bCanEverTick = false;

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

	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] BeginPlay 호출됨. %.1f초 뒤에 폭발 예정"), ExplosionDelay);

	// 일정 시간 후 폭발 예약
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AExplosive_Item::Explode, ExplosionDelay, false);
}

void AExplosive_Item::Explode()
{
	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 폭발 발생! 위치: %s"), *GetActorLocation().ToString());

	// 디버그 구체 시각화
	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 16, FColor::Red, false, 2.0f);

	// 폭발 범위 안의 캐릭터 찾기
	TArray<AActor*> OverlappedActors;
	ExplosionArea->GetOverlappingActors(OverlappedActors, ACharacter::StaticClass());

	for (AActor* Actor : OverlappedActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			// 넉백 방향 계산
			FVector Direction = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector KnockbackForce = Direction * KnockbackStrength;

			// 디버그용 라인 표시
			DrawDebugLine(GetWorld(), GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 2.0f);

			// 캐릭터에 넉백 적용
			Character->LaunchCharacter(KnockbackForce, true, true);
			UE_LOG(LogTemp, Log, TEXT("[Explosive_Item] 캐릭터 %s 에게 넉백 적용됨. 힘: %.1f"), *Character->GetName(), KnockbackStrength);

			// 데미지 적용
			UGameplayStatics::ApplyDamage(Character, Damage, GetInstigatorController(), this, nullptr);
			UE_LOG(LogTemp, Log, TEXT("[Explosive_Item] 캐릭터 %s 에게 데미지 %.1f 적용됨."), *Character->GetName(), Damage);
		}
	}

	// 폭발 후 자신 제거
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("[Explosive_Item] 폭발 아이템 제거됨."));
}