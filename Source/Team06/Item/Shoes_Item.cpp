#include "Item/Shoes_Item.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AShoes_Item::AShoes_Item()
{
    PrimaryActorTick.bCanEverTick = false;
    bAutoDestroyOnTrigger = false;
}

void AShoes_Item::BeginPlay()
{
    Super::BeginPlay();
}

void AShoes_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
    Super::TriggerEffect_Implementation(OverlappedActor);

    APlayerBase* Player = Cast<APlayerBase>(OverlappedActor);
    if (!Player || !Player->GetCharacterMovement())
        return;

    if (bIsBuffActive)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandle,
            [this, Player]() mutable
            {
                Player->GetCharacterMovement()->MaxWalkSpeed = StoredOrigSpeed;
                Player->GetCharacterMovement()->JumpZVelocity = StoredOrigJump;
                Multicast_ResetBootsBuff(Player, StoredOrigSpeed, StoredOrigJump);
                Multicast_StopLoopEffect();
                StopLoopSound();
                Destroy();
            },
            Duration,
            false
        );
        return;
    }

    bIsBuffActive = true;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);

    StoredOrigSpeed = Player->GetCharacterMovement()->MaxWalkSpeed;
    StoredOrigJump = Player->GetCharacterMovement()->JumpZVelocity;

    float BoostedSpeed = StoredOrigSpeed + SpeedBoost;
    float BoostedJump = StoredOrigJump + JumpBoost;
    Player->GetCharacterMovement()->MaxWalkSpeed = BoostedSpeed;
    Player->GetCharacterMovement()->JumpZVelocity = BoostedJump;
    Multicast_ApplyBootsBuff(Player, BoostedSpeed, BoostedJump);

    if (LoopSound)
        LoopSoundComp = UGameplayStatics::SpawnSoundAttached(
            LoopSound, Player->GetRootComponent(),
            NAME_None, FVector::ZeroVector,
            EAttachLocation::KeepRelativeOffset, true
        );
    Multicast_PlayLoopEffect(Player);

    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, Player]() mutable
        {
            Player->GetCharacterMovement()->MaxWalkSpeed = StoredOrigSpeed;
            Player->GetCharacterMovement()->JumpZVelocity = StoredOrigJump;
            Multicast_ResetBootsBuff(Player, StoredOrigSpeed, StoredOrigJump);
            Multicast_StopLoopEffect();
            StopLoopSound();
            Destroy();
        },
        Duration,
        false
    );
}


void AShoes_Item::StopLoopSound()
{
    if (LoopSoundComp && LoopSoundComp->IsPlaying())
    {
        LoopSoundComp->FadeOut(0.5f, 0.0f);
        LoopSoundComp = nullptr;
    }
}

void AShoes_Item::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    StopLoopSound();
}

void AShoes_Item::Multicast_PlayLoopEffect_Implementation(APlayerBase* Player)
{
    if (!Player) return;
}

void AShoes_Item::Multicast_StopLoopEffect_Implementation()
{
}

void AShoes_Item::Multicast_ApplyBootsBuff_Implementation(APlayerBase* Player, float NewSpeed, float NewJump)
{
    if (Player && Player->GetCharacterMovement())
    {
        Player->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
        Player->GetCharacterMovement()->JumpZVelocity = NewJump;
    }
}

void AShoes_Item::Multicast_ResetBootsBuff_Implementation(APlayerBase* Player, float OrigSpeed, float OrigJump)
{
    if (Player && Player->GetCharacterMovement())
    {
        Player->GetCharacterMovement()->MaxWalkSpeed = OrigSpeed;
        Player->GetCharacterMovement()->JumpZVelocity = OrigJump;
    }
}