#include "Obstacle/BlizzardArea.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DamageEvents.h"
#include "BonfireArea.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "EngineUtils.h" 

ABlizzardArea::ABlizzardArea()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    DamageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageArea"));
    SetRootComponent(DamageArea);
    DamageArea->SetCollisionProfileName(TEXT("Trigger"));
    DamageArea->SetGenerateOverlapEvents(true);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    VisualMesh->SetupAttachment(DamageArea);
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABlizzardArea::BeginPlay()
{
    Super::BeginPlay();
    if (HasAuthority())
    {
        DamageArea->OnComponentBeginOverlap.AddDynamic(this, &ABlizzardArea::OnDamageAreaBeginOverlap);
        DamageArea->OnComponentEndOverlap.AddDynamic(this, &ABlizzardArea::OnDamageAreaEndOverlap);

        if (ObstacleDataTable)
        {
            const FObstacleDataRow* Row = ObstacleDataTable->FindRow<FObstacleDataRow>(ObstacleRowName, TEXT("BlizzardArea"));
            ApplyObstacleData(Row);

            GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ABlizzardArea::DealDamageToPlayers, BlizzardTickInterval, true);
        }
    }
}

void ABlizzardArea::ApplyObstacleData(const FObstacleDataRow* Row)
{
    if (!Row || Row->ObstacleType != "BlizzardArea") return;

    BlizzardTickInterval = Row->BlizzardTickInterval;
    BlizzardDamage = Row->BlizzardDamage;
}

void ABlizzardArea::OnDamageAreaBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        OverlappingPlayers.Add(OtherActor);
    }
}

void ABlizzardArea::OnDamageAreaEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        OverlappingPlayers.Remove(OtherActor);
    }
}

void ABlizzardArea::DealDamageToPlayers()
{
    for (AActor* Player : OverlappingPlayers)
    {
        bool bInSafeZone = false;

        for (TActorIterator<ABonfireArea> It(GetWorld()); It; ++It)
        {
            if (It->IsPlayerInside(Player))
            {
                bInSafeZone = true;
                break;
            }
        }

        if (!bInSafeZone)
        {
            if (APlayerBase* PC = Cast<APlayerBase>(Player))
            {
                FDamageEvent DamageEvent;
                PC->TakeDamage(BlizzardDamage, DamageEvent, nullptr, this);
            }
        }
    }
}
