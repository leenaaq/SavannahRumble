#include "Player/Component/FlagActor.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

AFlagActor::AFlagActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
	SetRootComponent(FlagMesh);

	IdleTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("IdleTrigger"));
	IdleTrigger->SetupAttachment(RootComponent);
	IdleTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	IdleTrigger->SetGenerateOverlapEvents(true);
	IdleTrigger->SetSphereRadius(100.f);
	IdleTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFlagActor::OnIdleTriggerBegin);

	GameTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GameTrigger"));
	GameTrigger->SetupAttachment(RootComponent);
	GameTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GameTrigger->SetGenerateOverlapEvents(false);
	GameTrigger->SetCapsuleSize(50.f, 100.f);
	GameTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFlagActor::OnGameTriggerBegin);
}

void AFlagActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetFlagState(EFlagState::Idle);
	}
}

void AFlagActor::SetFlagState(EFlagState NewState)
{
	if (!HasAuthority())
	{
		return;
	}

	FlagState = NewState;

	if (FlagState == EFlagState::Idle)
	{
		IdleTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		IdleTrigger->SetGenerateOverlapEvents(true);
		GameTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GameTrigger->SetGenerateOverlapEvents(false);
	}
	else
	{
		IdleTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IdleTrigger->SetGenerateOverlapEvents(false);
		GameTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GameTrigger->SetGenerateOverlapEvents(true);
	}
}

void AFlagActor::OnIdleTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || FlagState != EFlagState::Idle || !OtherActor) return;

	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (!Character || !Character->GetPlayerState()) return;

	CurrentOwnerName = Character->GetPlayerState()->GetPlayerName();
	AttachToPlayer(Character);
	SetFlagState(EFlagState::Active);
}

void AFlagActor::OnGameTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || FlagState != EFlagState::Active || !bCanSwap || !OtherActor) return;

	ACharacter* NewChar = Cast<ACharacter>(OtherActor);
	if (!NewChar || !NewChar->GetPlayerState()) return;

	const FString& NewName = NewChar->GetPlayerState()->GetPlayerName();
	if (NewName == CurrentOwnerName) return;

	CurrentOwnerName = NewName;
	AttachToPlayer(NewChar);

	bCanSwap = false;
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AFlagActor::ResetSwapCooldown, 1.0f, false);
}

void AFlagActor::AttachToPlayer(AActor* NewOwner)
{
	ACharacter* Char = Cast<ACharacter>(NewOwner);
	if (Char && Char->GetMesh())
	{
		AttachToComponent(Char->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
		SetOwner(Char);
	}
}

void AFlagActor::ResetSwapCooldown()
{
	bCanSwap = true;
}
