#include "Item2/Trap/StickyZoneActor.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item2/Base/RangeItemDataRow.h"

AStickyZoneActor::AStickyZoneActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ZoneTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneTrigger"));
    SetRootComponent(ZoneTrigger);
    ZoneTrigger->InitSphereRadius(100.f);
    ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ZoneTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZoneTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStickyZoneActor::OnZoneBeginOverlap);
    ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &AStickyZoneActor::OnZoneEndOverlap);

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    VisualMesh->SetupAttachment(RootComponent);
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(RootComponent);
    Decal->DecalSize = FVector(100.f, 200.f, 200.f);
    Decal->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
}

void AStickyZoneActor::BeginPlay()
{
    Super::BeginPlay();

    if (DataRowHandle.DataTable)
    {
        const FRangeItemDataRow* Row = DataRowHandle.GetRow<FRangeItemDataRow>("StickyZone");
        if (Row)
        {
            SpeedMultiplier = Row->SpeedMultiplier;
            JumpMultiplier = Row->JumpMultiplier;
        }
    }
}

void AStickyZoneActor::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    APlayerBase* Player = Cast<APlayerBase>(OtherActor);
    if (Player && !AffectedPlayers.Contains(Player))
    {
        ApplySlowEffect(Player);
        AffectedPlayers.Add(Player);
    }
}

void AStickyZoneActor::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!HasAuthority()) return;

    APlayerBase* Player = Cast<APlayerBase>(OtherActor);
    if (Player && AffectedPlayers.Contains(Player))
    {
        RemoveSlowEffect(Player);
        AffectedPlayers.Remove(Player);
    }
}

void AStickyZoneActor::ApplySlowEffect(APlayerBase* Player)
{
    if (!IsValid(Player)) return;

    if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
    {
        if (MovementBackupMap.Contains(Player)) return;

        FMovementBackup Backup;
        Backup.OriginalSpeed = MoveComp->MaxWalkSpeed;
        Backup.OriginalJump = MoveComp->JumpZVelocity;
        MovementBackupMap.Add(Player, Backup);

        MoveComp->MaxWalkSpeed *= SpeedMultiplier;
        MoveComp->JumpZVelocity *= JumpMultiplier;

            *Player->GetName(), MoveComp->MaxWalkSpeed, MoveComp->JumpZVelocity);

        float EffectiveDuration = 0.f;
        if (DataRowHandle.DataTable)
        {
            const FRangeItemDataRow* Row = DataRowHandle.GetRow<FRangeItemDataRow>("StickyZone");
            if (Row)
            {
                EffectiveDuration = Row->SlowDuration;
            }
        }
        if (EffectiveDuration > 0.f)
        {
            FTimerHandle TimerHandle;
            FTimerDelegate TimerDel;
            TimerDel.BindLambda([this, Player]() {
                if (IsValid(Player) && Player->GetCharacterMovement() && MovementBackupMap.Contains(Player))
                {
                    const FMovementBackup& BackupRef = MovementBackupMap[Player];
                    Player->GetCharacterMovement()->MaxWalkSpeed = BackupRef.OriginalSpeed;
                    Player->GetCharacterMovement()->JumpZVelocity = BackupRef.OriginalJump;
                    MovementBackupMap.Remove(Player);
                }
                });

            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, EffectiveDuration, false);
        }
    }
}

void AStickyZoneActor::RemoveSlowEffect(APlayerBase* Player)
{
    if (!IsValid(Player)) return;

    if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
    {
        if (MovementBackupMap.Contains(Player))
        {
            const FMovementBackup& Backup = MovementBackupMap[Player];
            MoveComp->MaxWalkSpeed = Backup.OriginalSpeed;
            MoveComp->JumpZVelocity = Backup.OriginalJump;

            MovementBackupMap.Remove(Player);
        }
    }
}
