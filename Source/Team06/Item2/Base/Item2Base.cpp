#include "Item2/Base/Item2Base.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DataTable.h"
#include "Engine/DamageEvents.h"
#include "Item2/Base/RangeItemDataRow.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

AItem2Base::AItem2Base()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetUsingAbsoluteScale(false);
	SetRootComponent(MeshComp);

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

void AItem2Base::OnRespawned_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[ItemBase] 아이템 리스폰됨: %s"), *GetName());
}

void AItem2Base::ActivateParticle1()
{
	if (HasAuthority()) MulticastActivateParticle1();
	else ServerActivateParticle1();
}
void AItem2Base::ServerActivateParticle1_Implementation()
{
	MulticastActivateParticle1();
}
void AItem2Base::MulticastActivateParticle1_Implementation()
{
	if (!ParticleEffect1) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect1, GetActorTransform());
}
void AItem2Base::ActivateParticle2()
{
	if (HasAuthority()) MulticastActivateParticle2();
	else ServerActivateParticle2();
}
void AItem2Base::ServerActivateParticle2_Implementation()
{
	MulticastActivateParticle2();
}
void AItem2Base::MulticastActivateParticle2_Implementation()
{
	if (!ParticleEffect2) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect2, GetActorTransform());
}
void AItem2Base::ActivateParticle3()
{
	if (HasAuthority()) MulticastActivateParticle3();
	else ServerActivateParticle3();
}
void AItem2Base::ServerActivateParticle3_Implementation()
{
	MulticastActivateParticle3();
}
void AItem2Base::MulticastActivateParticle3_Implementation()
{
	if (!ParticleEffect3) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect3, GetActorTransform());
}
void AItem2Base::ActivateNiagara1()
{
	if (HasAuthority()) MulticastActivateNiagara1();
	else ServerActivateNiagara1();
}
void AItem2Base::ServerActivateNiagara1_Implementation()
{
	MulticastActivateNiagara1();
}
void AItem2Base::MulticastActivateNiagara1_Implementation()
{
	if (!NiagaraEffect1) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect1, GetActorLocation());
}
void AItem2Base::ActivateNiagara2()
{
	if (HasAuthority()) MulticastActivateNiagara2();
	else ServerActivateNiagara2();
}
void AItem2Base::ServerActivateNiagara2_Implementation()
{
	MulticastActivateNiagara2();
}
void AItem2Base::MulticastActivateNiagara2_Implementation()
{
	if (!NiagaraEffect2) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect2, GetActorLocation());
}
void AItem2Base::ActivateNiagara3()
{
	if (HasAuthority()) MulticastActivateNiagara3();
	else ServerActivateNiagara3();
}
void AItem2Base::ServerActivateNiagara3_Implementation()
{
	MulticastActivateNiagara3();
}
void AItem2Base::MulticastActivateNiagara3_Implementation()
{
	if (!NiagaraEffect3) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect3, GetActorLocation());
}
void AItem2Base::PlaySound1()
{
	if (HasAuthority()) MulticastPlaySound1();
	else ServerPlaySound1();
}
void AItem2Base::ServerPlaySound1_Implementation()
{
	MulticastPlaySound1();
}
void AItem2Base::MulticastPlaySound1_Implementation()
{
	if (!SoundEffect1) return;
	UGameplayStatics::PlaySoundAtLocation(this, SoundEffect1, GetActorLocation());
}
void AItem2Base::PlaySound2()
{
	if (HasAuthority()) MulticastPlaySound2();
	else ServerPlaySound2();
}
void AItem2Base::ServerPlaySound2_Implementation()
{
	MulticastPlaySound2();
}
void AItem2Base::MulticastPlaySound2_Implementation()
{
	if (!SoundEffect2) return;
	UGameplayStatics::PlaySoundAtLocation(this, SoundEffect2, GetActorLocation());
}
void AItem2Base::PlaySound3()
{
	if (HasAuthority()) MulticastPlaySound3();
	else ServerPlaySound3();
}
void AItem2Base::ServerPlaySound3_Implementation()
{
	MulticastPlaySound3();
}
void AItem2Base::MulticastPlaySound3_Implementation()
{
	if (!SoundEffect3) return;
	UGameplayStatics::PlaySoundAtLocation(this, SoundEffect3, GetActorLocation());
}
