#include "Bat_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ABat_Item::ABat_Item()
{	
	PrimaryActorTick.bCanEverTick = false;

	if (CollisionComp)
	{
		CollisionComp->SetCollisionProfileName(TEXT("PhysicsActor"));
		CollisionComp->SetNotifyRigidBodyCollision(true);
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
	// 방망이만의 추가 로직 (예: 더 강한 데미지 적용)
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
}

bool ABat_Item::ServerThrowItem_Validate(FVector Direction)
{
	return true;
}

void ABat_Item::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	if (!HasAuthority() || !OtherActor || OtherActor == this) return;

	ACharacter* HitCharacter = Cast<ACharacter>(OtherActor);
	if (HitCharacter)
	{
		FVector KnockbackDir = (HitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector KnockbackForce = KnockbackDir * 1000.0f;

		// 여기에 데미지 및 넉백 처리 함수 호출 가능
		UGameplayStatics::ApplyDamage(HitCharacter, 30.0f, GetInstigatorController(), this, nullptr);

		if (HitCharacter->GetCharacterMovement())
		{
			HitCharacter->LaunchCharacter(KnockbackForce, true, true);
		}
	}
}