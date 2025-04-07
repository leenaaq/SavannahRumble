#include "PlayerBase.h"
#include "Net/UnrealNetwork.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	UpdateStatsFromDataTable();
}

// 데이터 테이블 업데이트
void APlayerBase::UpdateStatsFromDataTable()
{
	if (StatsRowHandle.DataTable)
	{
		static const FString ContextString(TEXT("Player"));
		FPlayerStats* Stats = StatsRowHandle.DataTable->FindRow<FPlayerStats>(StatsRowHandle.RowName, ContextString);
		if (Stats)
		{
			PlayerStats = *Stats;
		}
	}
}

// 피격 함수
float APlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsStunned)
	{
		return 0.f;
	}

	float OldHealth = GetHealth();
	float NewHealth = FMath::Clamp(OldHealth - DamageAmount, 0.f, GetMaxHealth());
	SetHealth(NewHealth);

	UE_LOG(LogTemp, Log, TEXT("Attack %f, HP %f -> %f"), DamageAmount, OldHealth, NewHealth);

	if (NewHealth <= 0.f)
	{
		bIsStunned = true;
		OnStunned();
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayerBase::OnStunned()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->StopAllMontages(0.f);
		}
		MeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		MeshComp->Stop();

		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->SetSimulatePhysics(true);
		MeshComp->bBlendPhysics = true;
	}
}


void APlayerBase::OnRep_bIsStunned()
{
	if (bIsStunned)
	{
		OnStunned();
	}
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerBase, bIsStunned);
}