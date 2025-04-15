// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/AICharacter.h"
#include "AI/System/AIC_Enemy.h"
#include "Components/SphereComponent.h"
#include "Player/Component/FlagActor.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIC_Enemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DetectorRadius = 150.f;

	DetectingFlagCollision = CreateDefaultSubobject<USphereComponent>(TEXT("FlagDetector"));
	DetectingFlagCollision->InitSphereRadius(DetectorRadius);
	DetectingFlagCollision->SetupAttachment(GetRootComponent());

	DetectingFlagCollision->OnComponentBeginOverlap.AddDynamic(this, &AAICharacter::OnOverlapBegin);
}

void AAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if(!MyFlag)
	{
		AFlagActor* Flag = Cast<AFlagActor>(OtherActor);
		if (IsValid(Flag))
		{
			AAIC_Enemy* AIC = Cast<AAIC_Enemy>(AIControllerClass);
			if (AIC)
			{
				Flag->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
				Flag->SetOwner(this);

				MyFlag = Flag;
				SetbHasFlag(true);

				AIC->SetChaseMode(false);
			}
		}
	}
	else
	{
		APlayerBase* OtherPlayer = Cast<APlayerBase>(OtherActor);
		if (IsValid(OtherPlayer))
		{
			if (IsValid(OtherPlayer->GetMesh()))
			{
				AAIC_Enemy* AIC = Cast<AAIC_Enemy>(AIControllerClass);
				if (AIC)
				{
					MyFlag->AttachToComponent(OtherPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachSocketName);
					SetOwner(OtherPlayer);

					MyFlag = nullptr;
					SetbHasFlag(false);

					AIC->SetChaseMode(true);
				}
			}
		}
	}
}
