#include "Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::ServerApplyDamageAndKnockback_Implementation(AActor* Target, float Damage, float KnockbackForce)
{
	if (!Target) return;

	// 데미지 적용
	UGameplayStatics::ApplyDamage(Target, Damage, GetInstigatorController(), this, nullptr);

	// 넉백 적용
	ACharacter* HitCharacter = Cast<ACharacter>(Target);
	if (HitCharacter)
	{
		UCharacterMovementComponent* Movement = HitCharacter->GetCharacterMovement();
		if (Movement)
		{
			FVector KnockbackDir = (HitCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector Knockback = KnockbackDir * KnockbackForce;

			Movement->Launch(Knockback);
		}
	}
}

bool AItem::ServerApplyDamageAndKnockback_Validate(AActor* Target, float Damage, float KnockbackForce)
{
	return true;
}

void AItem::ServerUseItem_Implementation(AActor* Target)
{
	// 아이템 사용 로직 (자식 클래스에서 오버라이드 가능)
}

void AItem::ServerThrowItem_Implementation(FVector Direction)
{
	if (RootComponent)
	{
		UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(RootComponent);
		if (PrimitiveComp)
		{
			PrimitiveComp->AddImpulse(Direction * 1000.0f, NAME_None, true);
		}
	}
}

void AItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌 처리 로직 (자식 클래스에서 오버라이드 가능)
}

