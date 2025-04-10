#include "Item/EquipableItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

AEquipableItem::AEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetNotifyRigidBodyCollision(true);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(ItemMesh);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	OnActorHit.AddDynamic(this, &AEquipableItem::HandleImpact);
}

void AEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AEquipableItem::ServerUseItem_Implementation(AActor* Target)
{
	// 자식에서 오버라이드
}

void AEquipableItem::ServerThrowItem_Implementation(FVector Direction)
{
	if (ItemMesh)
	{
		bWasThrown = true; // 던졌다고 표시
		ItemMesh->AddImpulse(Direction * 1000.f, NAME_None, true);
	}
}

void AEquipableItem::ServerApplyDamageAndKnockback_Implementation(AActor* Target, float Damage, float KnockbackForce)
{
	if (!Target || !HasAuthority()) return;

	UGameplayStatics::ApplyDamage(Target, Damage, GetInstigatorController(), this, nullptr);

	if (UPrimitiveComponent* TargetComp = Cast<UPrimitiveComponent>(Target->GetRootComponent()))
	{
		if (TargetComp->IsSimulatingPhysics())
		{
			FVector KnockDir = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			TargetComp->AddImpulse(KnockDir * KnockbackForce, NAME_None, true);
		}
	}
}

bool AEquipableItem::ServerApplyDamageAndKnockback_Validate(AActor* Target, float Damage, float KnockbackForce)
{
	return true;
}

void AEquipableItem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHasLanded || !bWasThrown) return;

	bHasLanded = true;

	GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, this, &AEquipableItem::OnItemLanded, ActivationDelayAfterLanding, false);
}

void AEquipableItem::HandleImpact(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	OnHit(nullptr, OtherActor, nullptr, NormalImpulse, Hit);
}

void AEquipableItem::OnItemLanded_Implementation()
{
	if (bIsActivated) return;

	ActivateEffect();
	bIsActivated = true;
}

void AEquipableItem::ActivateEffect_Implementation()
{
	PlayItemEffects(GetActorLocation()); // 부모 기능 호출
}

void AEquipableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEquipableItem, bHasLanded);
	DOREPLIFETIME(AEquipableItem, bWasThrown);
	DOREPLIFETIME(AEquipableItem, bIsActivated);
}