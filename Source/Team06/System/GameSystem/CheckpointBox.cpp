// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSystem/CheckpointBox.h"
#include "Components/BoxComponent.h"
#include "Player/Controller/PCController_GamePlay.h"
#include "System/GameSystem/MyT6GSB_GL_Mountain.h"
#include "GameFramework/Character.h"

// Sets default values
ACheckpointBox::ACheckpointBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(300.f, 300.f, 300.f));
    CollisionBox->SetCollisionProfileName(TEXT("Trigger"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointBox::OnOverlapBegin);

    CheckPointIndex = 0;
    CheckPointLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void ACheckpointBox::BeginPlay()
{
    Super::BeginPlay();

    if (CheckPointLocation.IsZero())
    {
        CheckPointLocation = GetActorLocation();
    }
}

void ACheckpointBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* OverlappingCharacter = Cast<ACharacter>(OtherActor);
    if (!OverlappingCharacter) return;

    APCController_GamePlay* PC = Cast<APCController_GamePlay>(OverlappingCharacter->GetController());
    
    if (!PC)
    {
        UE_LOG(LogTemp, Log, TEXT("Invalid PlayerController call by CheckBox"));
        return;
    }
    else
    {
        ServerHandleCheckpoint(PC, CheckPointIndex);
    }
}

void ACheckpointBox::ServerHandleCheckpoint_Implementation(APlayerController* PC, int32 ThisCheckpointIndex)
{
    AMyT6GSB_GL_Mountain* GS = GetWorld()->GetGameState<AMyT6GSB_GL_Mountain>();
    if (GS)
    {
        GS->UpdatePlayerCheckpoint(PC, ThisCheckpointIndex);
        UE_LOG(LogTemp, Log, TEXT("Server updated %s to checkpoint %d"), *PC->GetName(), ThisCheckpointIndex);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Invalid GSB call by CheckBox"));
    }
}


