#include "Bat_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"

ABat_Item::ABat_Item()
{	
	PrimaryActorTick.bCanEverTick = false;

	if (CollisionComp)
	{
		CollisionComp->SetSimulatePhysics(true);                     // 물리 적용
		CollisionComp->SetCollisionProfileName(TEXT("PhysicsActor")); // 충돌 설정
		CollisionComp->SetNotifyRigidBodyCollision(true);             // 충돌 감지 허용
		CollisionComp->OnComponentHit.AddDynamic(this, &ABat_Item::OnHit); // OnHit 바인딩
	}

}

void ABat_Item::BeginPlay()
{
	Super::BeginPlay();
}

void ABat_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABat_Item::ServerUseItem(AActor* Target)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		ServerUseItem_Implementation(Target);
	}
	else
	{
		ServerUseItem(Target);
	}
}

void ABat_Item::ServerUseItem_Implementation(AActor* Target)
{
	Super::ServerUseItem_Implementation(Target);

	ACharacter* HitCharacter = Cast<ACharacter>(Target);
	if (HitCharacter && HitCharacter->GetCharacterMovement())
	{
		// 데미지 + 넉백
		UGameplayStatics::ApplyDamage(HitCharacter, 30.0f, GetInstigatorController(), this, nullptr);

		FVector KnockbackDir = (HitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector KnockbackForce = KnockbackDir * 1000.0f;
		HitCharacter->LaunchCharacter(KnockbackForce, true, true);

		UE_LOG(LogTemp, Warning, TEXT("[Bat] 직접 사용: %s 데미지 + 넉백"), *HitCharacter->GetName());
	}


}

bool ABat_Item::ServerUseItem_Validate(AActor* Target)
{
	return true;
}

void ABat_Item::ServerThrowItem(FVector Direction)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		ServerThrowItem_Implementation(Direction);
	}
	else
	{
		Super::ServerThrowItem(Direction);
	}
}

void ABat_Item::ServerThrowItem_Implementation(FVector Direction)
{
	Super::ServerThrowItem_Implementation(Direction);
	// 방망이만의 추가 로직 가능
	UE_LOG(LogTemp, Log, TEXT("[BatItem] 방망이 던짐 → 충돌 시 효과 활성화"));
}

bool ABat_Item::ServerThrowItem_Validate(FVector Direction)
{
	return true;
}

void ABat_Item::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority() || !OtherActor || OtherActor == this || !bWasThrown) return;

	ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
	if (HitCharacter)
	{
		UGameplayStatics::ApplyDamage(HitCharacter, 20.0f, GetInstigatorController(), this, nullptr);

		// 간단한 래그돌 처리
		HitCharacter->GetMesh()->SetSimulatePhysics(true);
		HitCharacter->GetCharacterMovement()->DisableMovement();

		FTimerHandle RagdollTimer;
		GetWorld()->GetTimerManager().SetTimer(RagdollTimer, [HitCharacter]()
			{
				if (HitCharacter)
				{
					HitCharacter->GetMesh()->SetSimulatePhysics(false);
					HitCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					UE_LOG(LogTemp, Warning, TEXT("[Bat] %s 래그돌 해제됨"), *HitCharacter->GetName());
				}
			}, 1.5f, false);

		UE_LOG(LogTemp, Warning, TEXT("[Bat] 던진 아이템으로 충돌 → %s 데미지 + 래그돌"), *HitCharacter->GetName());
	}

	bWasThrown = false; // 한 번만 발동되도록
}