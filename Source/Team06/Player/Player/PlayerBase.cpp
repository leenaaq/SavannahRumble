#include "PlayerBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/Component/ItemManagerComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "System/UI/UW_PlayerNameText.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/PlayerState.h"
#include "Player/PlayerState/PlayerCharacterState.h"
#include "Player/Controller/PCController_GamePlay.h"

APlayerBase::APlayerBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemManager = CreateDefaultSubobject<UItemManagerComponent>(TEXT("ItemManager"));

	EquipItemMeshActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("EquipItemMeshActorComponent"));
	EquipItemMeshActorComponent->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));

	MuzzleComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(GetMesh(), TEXT("hand_r_socket"));

	PlayerNameWidgetComponent = CreateDefaultSubobject<UPlayerTextWidgetComponent>(TEXT("PlayerNameWidget"));
	PlayerNameWidgetComponent->SetupAttachment(GetRootComponent());
	PlayerNameWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	PlayerNameWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	PlayerNameWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));
	PhysAnimData.bIsLocalSimulation = true;
	PhysAnimData.OrientationStrength = 5000.f;
	PhysAnimData.AngularVelocityStrength = 500.f;
	PhysAnimData.PositionStrength = 5000.f;
	PhysAnimData.VelocityStrength = 500.f;
	PhysAnimData.MaxLinearForce = 500.f;
	PhysAnimData.MaxAngularForce = 500.f;

	PigMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PigMesh"));
	PigMeshComponent->SetupAttachment(GetMesh());
	PigMeshComponent->SetRelativeLocation(FVector::ZeroVector);
	PigMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
	PigMeshComponent->SetRelativeScale3D(FVector(1.f));

	FoxMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FoxMesh"));
	FoxMeshComponent->SetupAttachment(GetMesh());
	FoxMeshComponent->SetRelativeLocation(FVector::ZeroVector);
	FoxMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
	FoxMeshComponent->SetRelativeScale3D(FVector(1.f));

	WolfMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WolfMesh"));
	WolfMeshComponent->SetupAttachment(GetMesh());
	WolfMeshComponent->SetRelativeLocation(FVector::ZeroVector);
	WolfMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
	WolfMeshComponent->SetRelativeScale3D(FVector(1.f));

	DeerMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DeerMesh"));
	DeerMeshComponent->SetupAttachment(GetMesh());
	DeerMeshComponent->SetRelativeLocation(FVector::ZeroVector);
	DeerMeshComponent->SetRelativeRotation(FRotator::ZeroRotator);
	DeerMeshComponent->SetRelativeScale3D(FVector(1.f));
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	ValidateEssentialReferences();

	if (!HasAuthority())
	{
		ActiveRagdoll();
	}

	UpdateStatsFromDataTable();
	UpdateSkinVisibility();

	if (PlayerNameWidgetComponent)
	{
		PlayerNameWidgetComponent->InitWidget();

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
			FVector WidgetLoc = PlayerNameWidgetComponent->GetComponentLocation();
			FRotator LookRot = (CamLoc - WidgetLoc).Rotation();
			PlayerNameWidgetComponent->SetWorldRotation(LookRot);
		}
	}
}


void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 닉네임
	if (!bPlayerNameInitialized && PlayerNameWidgetComponent && GetPlayerState())
	{
		if (UUW_PlayerNameText* NameWidget = Cast<UUW_PlayerNameText>(PlayerNameWidgetComponent->GetUserWidgetObject()))
		{
			NameWidget->UpdatePlayerName();
			bPlayerNameInitialized = true;
		}
	}

	// UI 닉네임 카메라
	if (IsValid(PlayerNameWidgetComponent) && HasAuthority() == false)
	{
		APlayerController* LocalController = UGameplayStatics::GetPlayerController(this, 0);
		if (IsValid(LocalController))
		{
			APlayerCameraManager* CameraManager = LocalController->PlayerCameraManager;
			if (IsValid(CameraManager))
			{
				const FVector WidgetLocation = PlayerNameWidgetComponent->GetComponentLocation();
				const FVector CameraLocation = CameraManager->GetCameraLocation();

				const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);
				PlayerNameWidgetComponent->SetWorldRotation(LookAtRotation);
			}
		}
	}
}


void APlayerBase::ValidateEssentialReferences()
{
    if (!ItemManager)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerBase.cpp : ItemManagerComponent 확인"));
    }
    if (!StatsRowHandle.DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerBase.cpp : StatsRowHandle.DataTable 확인"));
    }
    if (!MuzzleComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerBase.cpp : MuzzleComponent 확인"));
    }
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

			if (UCharacterMovementComponent* Movement = GetCharacterMovement())
			{
				Movement->MaxWalkSpeed = PlayerStats.WalkSpeed;
				Movement->JumpZVelocity = PlayerStats.JumpHigh;
			}
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
	DeactivateActiveRagdoll();
	OnStunned(PlayerStats.StunDuration);
}

void APlayerBase::OnStunned(float StunTime)
{
	DeactivateActiveRagdoll();
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

	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		//Movement->StopMovementImmediately();
		//Movement->DisableMovement();
	}

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
		CapsuleComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}

	RemainingStunTime = StunTime;
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(
			RecoveryTimerHandle,
			this,
			&APlayerBase::MulticastRecoverFromStun,
			PlayerStats.StunDuration,
			false
		);
	}
}

void APlayerBase::MulticastRecoverFromStun_Implementation()
{
	RecoverFromStun();
}

void APlayerBase::RecoverFromStun()
{
	bIsStunned = false;
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	MeshComp->SetSimulatePhysics(false);
	MeshComp->bBlendPhysics = false;
	MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

	MeshComp->AttachToComponent(
		GetCapsuleComponent(),
		FAttachmentTransformRules::SnapToTargetIncludingScale
	);

	StartRelLocation = MeshComp->GetRelativeLocation();
	StartRelRotation = MeshComp->GetRelativeRotation();
	TargetRelLocation = FVector(0.f, 0.f, -90.f);

	TargetRelRotation = FRotator(
		TargetRelRotation.Pitch,
		StartRelRotation.Yaw,
		TargetRelRotation.Roll
	);
	MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if (UAnimInstance* AnimInst = MeshComp->GetAnimInstance())
	{
		bool bFaceUp = FVector::DotProduct(
			MeshComp->GetUpVector(),
			FVector::UpVector
		) > 0.f;
		UAnimMontage* ChosenMontage =
			bFaceUp ? GetupForwardMontage : GetupBackwardMontage;

		if (ChosenMontage)
		{
			AnimInst->Montage_Play(
				ChosenMontage,
				1.f,
				EMontagePlayReturnType::MontageLength,
				0.f,
				true
			);

			InterpDuration = 0.25f;
			InterpElapsed = 0.f;
			GetWorld()->GetTimerManager().SetTimer(
				TransformInterpTimerHandle,
				this,
				&APlayerBase::UpdateTransformInterp,
				0.01f,
				true
			);
		}
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetHealth(GetMaxHealth());
}

void APlayerBase::UpdateTransformInterp()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		GetWorld()->GetTimerManager().ClearTimer(TransformInterpTimerHandle);
		return;
	}

	InterpElapsed += 0.01f;
	float Alpha = FMath::Clamp(InterpElapsed / InterpDuration, 0.f, 1.f);
	FVector NewLoc = FMath::Lerp(StartRelLocation, TargetRelLocation, Alpha);
	MeshComp->SetRelativeLocation(NewLoc);
	float NewPitch = FMath::Lerp(StartRelRotation.Pitch, TargetRelRotation.Pitch, Alpha);
	float NewRoll = FMath::Lerp(StartRelRotation.Roll, TargetRelRotation.Roll, Alpha);
	float FixedYaw = StartRelRotation.Yaw;
	MeshComp->SetRelativeRotation(FRotator(NewPitch, FixedYaw, NewRoll));

	if (Alpha >= 1.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TransformInterpTimerHandle);
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
	DOREPLIFETIME(APlayerBase, RemainingStunTime);
	DOREPLIFETIME(APlayerBase, PlayerStats);
	DOREPLIFETIME(APlayerBase, SkinName);
}

void APlayerBase::OnRep_CurrentEquippedItemName()
{
	if (!ItemManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerBase.cpp : ItemManagerComponent 확인"));
		return;
	}
	ItemManager->UpdateItemVisibility(CurrentEquippedItemName);
}

void APlayerBase::ServerSetEquippedItemName_Implementation(FName NewItemName)
{
	CurrentEquippedItemName = NewItemName;

	if (!ItemManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerBase.cpp : ItemManagerComponent 확인"));
		return;
	}
	ItemManager->ServerEquipItem(NewItemName);
	ItemManager->UpdateItemVisibility(CurrentEquippedItemName);
}

bool APlayerBase::ServerSetEquippedItemName_Validate(FName NewItemName)
{
	return true;
}

//void APlayerBase::SetEquipItemMeshStatic(UStaticMesh* NewMesh)
//{
//	if (EquipItemChildActor)
//	{
//		AEquipItemMeshActor* EquipMeshActor = Cast<AEquipItemMeshActor>(EquipItemChildActor->GetChildActor());
//		if (EquipMeshActor)
//		{
//			EquipMeshActor->MeshComp->SetStaticMesh(NewMesh);
//			EquipMeshActor->MeshComp->SetVisibility(NewMesh != nullptr);
//			EquipMeshActor->MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
//		}
//	}
//}


// 사망 로직
void APlayerBase::ServerProcessDeath_Implementation(FVector RespawnLocation)
{
	bIsStunned = true;
	DeactivateActiveRagdoll();
	OnStunned();

	if (GetLifeCount() > 0)
	{
		SetLifeCount(GetLifeCount() - 1);
		UE_LOG(LogTemp, Log, TEXT("[PlayerBase] Character LifeCount %d. Respawn..."), GetLifeCount());

		if (!HasAuthority())
		{
			ActiveRagdoll();
		}
		// 레벨별 GetRespawnLocation 필요
		//RespawnCharacter(RespawnLocation);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[PlayerBase] Character LifeCount X. Death."));
	}
}

bool APlayerBase::ServerProcessDeath_Validate(FVector RespawnLocation)
{
	return true;
}


// 리스폰
void APlayerBase::RespawnCharacter_Implementation(FVector RespawnLocation)
{
	SetActorLocation(RespawnLocation);
	UE_LOG(LogTemp, Log, TEXT("[PlayerBase] Character Respawn : %s"), *RespawnLocation.ToString());
}

void APlayerBase::ActiveRagdoll()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		PhysicalAnimationComponent->SetSkeletalMeshComponent(MeshComp);
		PhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(
			TEXT("pelvis"), PhysAnimData, true);
		MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
		MeshComp->RecreatePhysicsState();
		MeshComp->SetAllBodiesBelowSimulatePhysics(TEXT("pelvis"), true, false);
	}
}


void APlayerBase::DeactivateActiveRagdoll()
{
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionProfileName(TEXT("CharacterMesh"));

		PhysicalAnimationComponent->SetSkeletalMeshComponent(nullptr);

		MeshComp->SetAllBodiesSimulatePhysics(false);
		MeshComp->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		MeshComp->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
		MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

		if (UAnimInstance* AnimInstance = MeshComp->GetAnimInstance())
		{
			AnimInstance->InitializeAnimation();
		}
	}
}

//----------------------------------- 스킨 -----------------------------------
void APlayerBase::UpdateSkinVisibility()
{
	PigMeshComponent->SetVisibility(SkinName == FName("Pig"));
	FoxMeshComponent->SetVisibility(SkinName == FName("Fox"));
	WolfMeshComponent->SetVisibility(SkinName == FName("Wolf"));
	DeerMeshComponent->SetVisibility(SkinName == FName("Deer"));
}

void APlayerBase::OnRep_SkinName()
{
	UpdateSkinVisibility();
}

bool APlayerBase::ServerSetSkinName_Validate(FName NewSkinName)
{
	return true;
}

void APlayerBase::ServerSetSkinName_Implementation(FName NewSkinName)
{
	SkinName = NewSkinName;
	UpdateSkinVisibility();
}

void APlayerBase::SetSkinName(FName NewSkinName)
{
	if (HasAuthority())
	{
		SkinName = NewSkinName;
		UpdateSkinVisibility();
	}
	else
	{
		ServerSetSkinName(NewSkinName);
	}
}
//--------------------------------------------------------------------------