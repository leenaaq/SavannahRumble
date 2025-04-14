#include "Item/ElectricField_Item.h"
#include "Player/Player/PlayerBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AElectricField_Item::AElectricField_Item()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ItemID = "BP_ElectricField_Item";
}

void AElectricField_Item::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("[ElectricField] BeginPlay 호출됨"));
}

void AElectricField_Item::OnItemPickedUp(AActor* OtherActor)
{
    if (HasAuthority())
    {
        if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
        {
            Player->ServerSetEquippedItemName(ItemID);
            UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 아이템 습득: %s"), *Player->GetName());
        }
    }
}

void AElectricField_Item::ServerUseItem_Implementation(AActor* Target)
{
    if (!HasAuthority() || bFieldActivated) return;

    bFieldActivated = true;
    UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 아이템 사용됨 → %f초 뒤 발동 대기"), ActivationDelay);

    GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, this, &AElectricField_Item::ActivateElectricField, ActivationDelay, false);
}

void AElectricField_Item::ActivateElectricField()
{
    if (!HasAuthority()) return;

    UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 자기장 발동 시작"));

    ApplyFieldEffect();
    Multicast_PlayFieldEffect();

    GetWorld()->GetTimerManager().SetTimer(DurationTimerHandle, this, &AElectricField_Item::DestroyItem, FieldDuration, false);
}

void AElectricField_Item::ApplyFieldEffect()
{
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        APlayerBase* Player = Cast<APlayerBase>(Actor);
        if (Player && Player->GetHealth() > 0 && FVector::Dist(Player->GetActorLocation(), GetActorLocation()) <= FieldRadius)
        {
            UE_LOG(LogTemp, Warning, TEXT("[ElectricField] %s → Ragdoll 적용"), *Player->GetName());

            Player->OnStunned(true); // APlayerBase 함수 필요
            FTimerHandle RestoreHandle;
            FTimerDelegate RestoreDelegate;

            RestoreDelegate.BindLambda([Player]()
                {
                    if (Player) Player->OnStunned(false);
                });

            GetWorld()->GetTimerManager().SetTimer(RestoreHandle, RestoreDelegate, FieldDuration, false);
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), FieldRadius, 16, FColor::Blue, false, FieldDuration);
}

void AElectricField_Item::Multicast_PlayFieldEffect_Implementation()
{
    if (FieldEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FieldEffect, GetActorLocation(), GetActorRotation(), FVector(1), true, true);
    }
}

void AElectricField_Item::DestroyItem()
{
    UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 자기장 종료 → 아이템 제거"));
    Destroy();
}
