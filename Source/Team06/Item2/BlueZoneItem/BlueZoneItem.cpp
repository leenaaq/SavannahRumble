#include "Item2/BlueZoneItem/BlueZoneItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Player/Player/PlayerBase.h"
#include "Engine/DamageEvents.h"

ABlueZoneItem::ABlueZoneItem()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp->SetSimulatePhysics(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
    MeshComp->SetIsReplicated(true);

    ZoneTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneTrigger"));
    ZoneTrigger->SetupAttachment(MeshComp);
    ZoneTrigger->InitSphereRadius(10.f);
    ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ZoneTrigger->SetCollisionProfileName(TEXT("Trigger"));
    ZoneTrigger->SetGenerateOverlapEvents(true);
    ZoneTrigger->SetIsReplicated(true);

    VisualSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualSphere"));
    VisualSphere->SetupAttachment(MeshComp);
    VisualSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    VisualSphere->SetVisibility(true);
    VisualSphere->SetHiddenInGame(false);

    bReplicates = true;
    SetReplicateMovement(true);

    ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABlueZoneItem::OnZoneBeginOverlap);
    ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &ABlueZoneItem::OnZoneEndOverlap);
}

void ABlueZoneItem::OnSpawn()
{
    Super::OnSpawn();

    if (!HasAuthority())
        return;


    if (ItemDataTable)
    {
        static const FString Ctx(TEXT("RangeRow"));
        if (const FRangeItemDataRow* Row = ItemDataTable->FindRow<FRangeItemDataRow>(ItemRowName, Ctx))
        {
            DamagePerSecond = Row->DamagePerSecond;
            MaxRadius = Row->MaxRadius;
            ExpansionTime = Row->ExpansionTime;
        }
    }

    CurrentRadius = 10.f;
    ZoneTrigger->SetSphereRadius(CurrentRadius);
    UpdateVisualSphere();

    if (HasAuthority())
    {
        GetWorld()->GetTimerManager().SetTimer(
            ActivationTimerHandle,
            this,
            &ABlueZoneItem::ActivateZone,
            ActivationDelay,
            false
        );
    }
}

void ABlueZoneItem::ActivateZone()
{
    if (!HasAuthority()) return;

    bZoneActive = true;
    ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    GetWorld()->GetTimerManager().SetTimer(
        DamageTimerHandle,
        this,
        &ABlueZoneItem::ApplyZoneDamage,
        DamageInterval,
        true
    );
}

void ABlueZoneItem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bZoneActive || !HasAuthority()) return;

    ElapsedTime += DeltaSeconds;
    const float Alpha = FMath::Clamp(ElapsedTime / ExpansionTime, 0.f, 1.f);
    CurrentRadius = FMath::Lerp(10.f, MaxRadius, Alpha);
    ZoneTrigger->SetSphereRadius(CurrentRadius);
    UpdateVisualSphere();

    if (ElapsedTime >= ExpansionTime)
    {
        GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
        Destroy();
    }
}

void ABlueZoneItem::UpdateVisualSphere()
{
    if (VisualSphere)
    {
        const float Scale = CurrentRadius / 10.f;
        VisualSphere->SetWorldScale3D(FVector(Scale));
        VisualSphere->SetWorldLocation(GetActorLocation());
    }
}

void ABlueZoneItem::ApplyZoneDamage()
{
    if (!HasAuthority()) return;

    const float Dmg = DamagePerSecond * DamageInterval;

    for (APlayerBase* Player : InsidePlayers)
    {
        if (!IsValid(Player)) continue;

        FDamageEvent Ev;
        Player->TakeDamage(Dmg, Ev, nullptr, this);
    }
}

void ABlueZoneItem::OnZoneBeginOverlap(UPrimitiveComponent*, AActor* OtherActor,
    UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
    {
        InsidePlayers.Add(Player);
    }
}

void ABlueZoneItem::OnZoneEndOverlap(UPrimitiveComponent*, AActor* OtherActor,
    UPrimitiveComponent*, int32)
{
    if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
    {
        InsidePlayers.Remove(Player);
    }
}

void ABlueZoneItem::OnRep_CurrentRadius()
{
    ZoneTrigger->SetSphereRadius(CurrentRadius);
    UpdateVisualSphere();
}

void ABlueZoneItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABlueZoneItem, DamagePerSecond);
    DOREPLIFETIME(ABlueZoneItem, MaxRadius);
    DOREPLIFETIME(ABlueZoneItem, ExpansionTime);
    DOREPLIFETIME(ABlueZoneItem, CurrentRadius);
}
