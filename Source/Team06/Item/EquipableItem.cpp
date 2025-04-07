#include "EquipableItem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"



AEquipableItem::AEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(CollisionComp);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipableItem::ServerApplyDamageAndKnockback_Implementation(AActor* Target, float Damage, float KnockbackForce)
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

bool AEquipableItem::ServerApplyDamageAndKnockback_Validate(AActor* Target, float Damage, float KnockbackForce)
{
	return true;
}

void AEquipableItem::ServerUseItem_Implementation(AActor* Target)
{
	// 아이템 사용 로직 (자식 클래스에서 오버라이드 가능)
}

void AEquipableItem::ServerThrowItem_Implementation(FVector Direction)
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

void AEquipableItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 충돌 처리 로직 (자식 클래스에서 오버라이드 가능)
}

