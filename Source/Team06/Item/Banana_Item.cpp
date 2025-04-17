#include "Item/Banana_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Player/Player/PlayerBase.h" 
#include "Math/UnrealMathUtility.h"

void ABanana_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
    Super::TriggerEffect_Implementation(OverlappedActor);

    APlayerBase* Player = Cast<APlayerBase>(OverlappedActor);
    if (Player == nullptr)
    {
    
        return;
    }

    UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
    if (MoveComp == nullptr)
    {
     
        return;
    }

    if (SlipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlipSound, GetActorLocation());
        Destroy();
    }

    const float SavedGroundFriction = MoveComp->GroundFriction;
    const float SavedBrakingFactor = MoveComp->BrakingFrictionFactor;
    const float SavedDecelWalking = MoveComp->BrakingDecelerationWalking;

    MoveComp->GroundFriction = SavedGroundFriction * 0.2f;
    MoveComp->BrakingFrictionFactor = SavedBrakingFactor;
    MoveComp->BrakingDecelerationWalking = 1200.f;

    FVector Forward = Player->GetActorForwardVector();
    Forward.Z = 0.f;
    Forward.Normalize();

    const float MinSlideSpeed = 100.f;
    const float MaxSlideSpeed = 1500.f;
    const float SlideSpeed = FMath::Clamp(SlideForce, MinSlideSpeed, MaxSlideSpeed);

    Player->LaunchCharacter(Forward * SlideSpeed, true, true);

    FTimerHandle DisableMoveHandle;
    GetWorldTimerManager().SetTimer(
        DisableMoveHandle,
        [Player]()
        {
            if (IsValid(Player))
            {
                if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
                {
                    MoveComp->DisableMovement();
                    MoveComp->SetMovementMode(MOVE_None);
                    UE_LOG(LogTemp, Log, TEXT("Banana_Item: 이동 비활성화됨."));
                }
            }
        },
        0.15f,
        false
    );

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        [Player,SavedGroundFriction, SavedBrakingFactor, SavedDecelWalking]()
        {
            if (IsValid(Player) == false) { return; }

            if (UCharacterMovementComponent* MC = Player->GetCharacterMovement())
            {
                MC->GroundFriction = SavedGroundFriction;
                MC->BrakingFrictionFactor = SavedBrakingFactor;
                MC->BrakingDecelerationWalking = SavedDecelWalking;
            }

        },
        RagdollDuration,
        false);

    if (APlayerBase* PBase = Cast<APlayerBase>(Player))
    {
        if (PBase->HasAuthority())
        {
            const float BananaStunTime = 1.5f;
            PBase->OnStunned(BananaStunTime);
        }
    }

    // Destroy 호출해서 리스폰 트리거
Destroy();

}

// 인터페이스 함수 정의
void ABanana_Item::OnRespawned_Implementation()
{
  
    // 필요한 초기화 동작이 있다면 여기에
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}