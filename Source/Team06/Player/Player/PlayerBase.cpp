#include "PlayerBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/Component/ItemManagerComponent.h"
#include "Components/ChildActorComponent.h"
#include "../Component/EquipItemMeshActor.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemManager = CreateDefaultSubobject<UItemManagerComponent>(TEXT("ItemManager"));

	EquipItemChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("EquipItemChildActor"));
	EquipItemChildActor->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));
	EquipItemChildActor->SetChildActorClass(AEquipItemMeshActor::StaticClass());
	//EquipItemMesh->SetVisibility(false);
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
		static const FString ContextString(TEXT("PlayerStats"));
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

// 스턴 시 레그돌
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
	DOREPLIFETIME(APlayerBase, CurrentEquippedItemName);
}

void APlayerBase::OnRep_CurrentEquippedItemName()
{
	if (ItemManager)
	{
		ItemManager->UpdateItemVisibility(CurrentEquippedItemName);
	}
}

void APlayerBase::ServerSetEquippedItemName_Implementation(FName NewItemName)
{
	CurrentEquippedItemName = NewItemName;

	if (ItemManager)
	{
		ItemManager->UpdateItemVisibility(CurrentEquippedItemName);
	}
}

bool APlayerBase::ServerSetEquippedItemName_Validate(FName NewItemName)
{
	return true;
}

void APlayerBase::SetEquipItemMeshStatic(UStaticMesh* NewMesh)
{
	if (EquipItemChildActor)
	{
		AEquipItemMeshActor* EquipMeshActor = Cast<AEquipItemMeshActor>(EquipItemChildActor->GetChildActor());
		if (EquipMeshActor)
		{
			EquipMeshActor->MeshComp->SetStaticMesh(NewMesh);
			EquipMeshActor->MeshComp->SetVisibility(NewMesh != nullptr);
		}
	}
}