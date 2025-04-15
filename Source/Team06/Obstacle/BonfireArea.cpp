#include "Obstacle/BonfireArea.h"
#include "Player/Player/PlayerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"

ABonfireArea::ABonfireArea()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ProtectionZone = CreateDefaultSubobject<USphereComponent>(TEXT("ProtectionZone"));
    SetRootComponent(ProtectionZone);
    ProtectionZone->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ProtectionZone->SetSphereRadius(300.f);
}

void ABonfireArea::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        if (ObstacleDataTable)
        {
            const FObstacleDataRow* Row = ObstacleDataTable->FindRow<FObstacleDataRow>(ObstacleRowName, TEXT("Bonfire"));
            ApplyObstacleData(Row);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BonfireArea: ObstacleDataTable is null"));
        }
        if (HealInterval > 0.0f)
        {
            GetWorldTimerManager().SetTimer(HealTimerHandle, this, &ABonfireArea::HealPlayersInZone, HealInterval, true);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("BonfireArea: HealInterval is 0, timer not set"));
        }
    }
}

void ABonfireArea::ApplyObstacleData(const FObstacleDataRow* Row)
{
    if (Row)
    {
        if (Row->ObstacleType == "Bonfire")
        {
            HealInterval = Row->HealInterval;
            HealAmount = Row->HealAmount;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BonfireArea: DataTable row not found"));
    }
}

void ABonfireArea::HealPlayersInZone()
{
    TArray<AActor*> OverlappingActors;
    ProtectionZone->GetOverlappingActors(OverlappingActors, APlayerBase::StaticClass());

    for (AActor* Player : OverlappingActors)
    {
        if (APlayerBase* PC = Cast<APlayerBase>(Player))
        {
            float CurrentHealth = PC->GetHealth();
            float MaxHealth = PC->GetMaxHealth();

            if (CurrentHealth < MaxHealth)
            {
                float NewHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.f, MaxHealth);
                PC->SetHealth(NewHealth);
            }
        }
    }

}

bool ABonfireArea::IsPlayerInside(AActor* Player) const
{
    return ProtectionZone->IsOverlappingActor(Player);
}
