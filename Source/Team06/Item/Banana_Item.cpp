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

    ACharacter* Character = Cast<ACharacter>(OverlappedActor);
    if (Character == nullptr)
    {
    
        return;
    }

    UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement();
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

    FVector Forward = Character->GetActorForwardVector();
    Forward.Z = 0.f;
    Forward.Normalize();

    const float MinSlideSpeed = 100.f;
    const float MaxSlideSpeed = 1500.f;
    const float SlideSpeed = FMath::Clamp(SlideForce, MinSlideSpeed, MaxSlideSpeed);

    Character->LaunchCharacter(Forward * SlideSpeed, true, true);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        [Character, SavedGroundFriction, SavedBrakingFactor, SavedDecelWalking]()
        {
            if (IsValid(Character) == false) { return; }

            if (UCharacterMovementComponent* MC = Character->GetCharacterMovement())
            {
                MC->GroundFriction = SavedGroundFriction;
                MC->BrakingFrictionFactor = SavedBrakingFactor;
                MC->BrakingDecelerationWalking = SavedDecelWalking;
            }

        },
        RagdollDuration,
        false);

    if (APlayerBase* PBase = Cast<APlayerBase>(Character))
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