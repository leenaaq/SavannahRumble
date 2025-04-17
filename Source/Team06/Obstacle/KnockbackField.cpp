#include "KnockbackField.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Player/Player/PlayerBase.h"

AKnockbackField::AKnockbackField()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("KnockbackArea"));
	SetRootComponent(AreaSphere);
	AreaSphere->InitSphereRadius(KnockbackRadius);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AKnockbackField::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && bIsEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(KnockbackTimer, this, &AKnockbackField::TriggerKnockback, Interval, true);
	}
}

void AKnockbackField::TriggerKnockback()
{
	if (!bIsEnabled) return;

	TArray<AActor*> OverlappingActors;
	AreaSphere->GetOverlappingActors(OverlappingActors, APlayerBase::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		APlayerBase* Player = Cast<APlayerBase>(Actor);
		if (Player)
		{
			FVector Dir = Player->GetActorLocation() - GetActorLocation();
			Dir.Z = 0.f;
			Dir.Normalize();

			FVector Knockback = Dir * KnockbackForce;
			Player->LaunchCharacter(Knockback, true, true);
		}
	}

	Multicast_PlayKnockbackEffects();
}

void AKnockbackField::Multicast_PlayKnockbackEffects_Implementation()
{
	if (KnockbackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			KnockbackEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
}
