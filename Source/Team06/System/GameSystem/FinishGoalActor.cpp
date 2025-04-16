// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/FinishGoalActor.h"
#include "Components/BoxComponent.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "AI/System/AIC_Enemy.h"
// Sets default values
AFinishGoalActor::AFinishGoalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GoalCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalCollisionBox"));
	RootComponent = GoalCollisionBox;

	GoalCollisionBox->SetBoxExtent(FVector(200.f, 200.f, 300.f));
	GoalCollisionBox->SetCollisionProfileName(TEXT("Trigger"));
	GoalCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishGoalActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFinishGoalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFinishGoalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;
	if (!Pawn->GetController()) return;

	if (Pawn->GetController()->IsPlayerController())
	{
		APCController_GamePlay* PC = Cast<APCController_GamePlay>(Pawn->GetController());
		if (PC)
		{
			PC->ServerNotifyGoalReached();
		}
	}
	else
	{
		AAIController* AIC = Cast<AAIController>(Pawn->GetController());
		// AI의 경우
	}

}


