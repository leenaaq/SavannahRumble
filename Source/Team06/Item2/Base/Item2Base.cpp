#include "Item2Base.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DataTable.h"
#include "Engine/DamageEvents.h"
#include "Item2/Base/RangeItemDataRow.h"

AItem2Base::AItem2Base()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComp->InitialSpeed = 0.f;
	ProjectileMovementComp->MaxSpeed = 0.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;

	CurrentState = EItemState::Flying;

	Damage = 0.0f;
	StunDuration = 0.0f;
	ExplosionDelay = 0.0f;
	ExplosionRadius = 0.0f;
	KnockBackMultiplier = 0.0f;
	SpawnTime = 0.f;
}

void AItem2Base::BeginPlay()
{
	Super::BeginPlay();

	SpawnTime = GetWorld()->GetTimeSeconds();

	InitializeFromDataTable();
	OnSpawn();
}

void AItem2Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem2Base::OnSpawn()
{
	if (MeshComp)
	{
		MeshComp->SetSimulatePhysics(true);
	}
}

void AItem2Base::OnCollision(AActor* OtherActor)
{
	if (!OtherActor) return;

	if (OtherActor == GetInstigator())
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - SpawnTime < 1.0f)
		{
			return;
		}
	}

	if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
	{
		Player->OnStunned();
		ApplyEffect(Player);
	}
	TransitionToLanded();
}

void AItem2Base::TransitionToLanded()
{
	CurrentState = EItemState::Landed;
}

void AItem2Base::ApplyEffect(APlayerBase* AffectedPlayer)
{
	if (AffectedPlayer)
	{
		FDamageEvent DamageEvent;
		AffectedPlayer->TakeDamage(Damage, DamageEvent, nullptr, this);
	}
}

void AItem2Base::ServerHandleCollision_Implementation(AActor* OtherActor)
{
	OnCollision(OtherActor);
}

bool AItem2Base::ServerHandleCollision_Validate(AActor* OtherActor)
{
	return true;
}

void AItem2Base::InitializeFromDataTable()
{
	if (!ItemDataTable)
	{
		return;
	}

	static const FString ContextString(TEXT("ItemDataContext"));
	FRangeItemDataRow* RangeRow = ItemDataTable->FindRow<FRangeItemDataRow>(ItemRowName, ContextString);
	if (RangeRow)
	{
		Damage = RangeRow->Damage;
		StunDuration = RangeRow->StunDuration;
		ExplosionDelay = RangeRow->ExplosionDelay;
		ExplosionRadius = RangeRow->ExplosionRadius;
		KnockBackMultiplier = RangeRow->KnockBackMultiplier;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("데이터테이블에서 %s 행을 찾을 수 없음"), *ItemRowName.ToString());
	}
}
