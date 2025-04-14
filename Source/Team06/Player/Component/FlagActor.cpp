#include "Player/Component/FlagActor.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

#include "System/GameSystem/T6GSB_GL_FlagMap.h"
#include "Player/Controller/PCController_GamePlay.h"

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

	APawn* Character = Cast<APawn>(OtherActor);

	AController* NewController = Character->GetController();
	
	if (NewController)
	{
		OnServerFlagTriggered(Character,NewController);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Controller"));
	}


}

void AFlagActor::OnGameTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || FlagState != EFlagState::Active || !bCanSwap || !OtherActor) return;

	APawn* NewChar = Cast<APawn>(OtherActor);
	AController* NewController = NewChar->GetController();
	if (NewController)
	{
		OnServerFlagTriggered(NewChar, NewController);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Controller"));
	}	
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

void AFlagActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bCanSwap);
}

bool AFlagActor::OnServerFlagTriggered_Validate(APawn* NewOwnerPawn, AController* NewOwner)
{
	return (IsValid(NewOwnerPawn) && IsValid(NewOwner));
}

void AFlagActor::OnServerFlagTriggered_Implementation(APawn* NewOwnerPawn, AController* NewOwner)
{
	AT6GSB_GL_FlagMap* GSB_F = GetWorld()->GetGameState<AT6GSB_GL_FlagMap>();

	if (IsValid(GSB_F))
	{
		GSB_F->ServerChangeFlagOwner(NewOwner);

		AttachToPlayer(NewOwnerPawn);
		SetFlagState(EFlagState::Active);

		bCanSwap = false;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AFlagActor::ResetSwapCooldown, 1.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No GSB Founded at FlagActor"));
	}
}
